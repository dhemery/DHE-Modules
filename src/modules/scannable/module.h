#pragma once

#include "anchor.h"
#include "components/cxmath.h"
#include "config/common-config.h"
#include "config/curvature-config.h"
#include "config/duration-config.h"
#include "config/level-config.h"
#include "control-ids.h"
#include "controls/curvature-inputs.h"
#include "controls/duration-inputs.h"
#include "controls/level-inputs.h"
#include "generator.h"
#include "sequence-controller.h"
#include "step-controller.h"
#include "step-selector.h"

#include <engine/Module.hpp>
#include <jansson.h>

#include <iostream>

namespace dhe {
namespace scannable {

// Skew the progress::brightness ratio so that the "remaining" light stays
// fully lit for a little while during early progress, and the "completed"
// light reaches fully lit a little while before progress is complete.
static auto constexpr brightness_skew = 0.7F;
static auto constexpr brightness_range =
    Range{-brightness_skew, 1.F + brightness_skew};
static auto constexpr minimum_duration = short_duration_range.lower_bound();

template <typename P, typename I>
static inline auto duration(P const &duration_knob, P const &range_switch,
                            P const &multipler_knob, I const &multiplier_cv)
    -> float {
  auto const nominal_duration =
      dhe::selectable_duration(duration_knob, range_switch);
  auto const multiplier_rotation = dhe::rotation(multipler_knob, multiplier_cv);
  auto const nominal_multiplier = gain_range.scale(multiplier_rotation);
  auto const clamped_multiplier = gain_range.clamp(nominal_multiplier);
  auto const scaled_duration = nominal_duration * clamped_multiplier;
  auto const safe_duration = cx::max(minimum_duration, scaled_duration);
  return safe_duration;
}

template <typename P, typename I>
static inline auto level(P const &level_knob, P const &range_switch,
                         P const &multipler_knob, I const &multiplier_cv)
    -> float {
  auto const range = level_range(range_switch);
  auto const rotation = dhe::rotation_of(level_knob);
  auto const nominal_level = range.scale(rotation);
  auto const attenuation = dhe::rotation(multipler_knob, multiplier_cv);
  auto const attenuated_level = nominal_level * attenuation;
  return range.clamp(attenuated_level);
}

template <int N> class Module : public rack::engine::Module {
  using Input = InputIds<N>;
  using Light = LightIds<N>;
  using Output = OutputIds;
  using Param = ParamIds<N>;

public:
  Module() {
    config(Param::Count, Input::Count, Output::Count, Light::Count);

    configParam(Param::Length, 1.F, N, N, "Steps", "");
    config_level_range_switch(this, Param::LevelRange);

    for (auto step = 0; step < N; step++) {
      config_toggle<anchor_source_count>(this, Param::StartAnchorSource + step,
                                         "Start anchor source",
                                         {"Level", "A", "B", "C", "Out"}, 4);
      config_level_knob(this, Param::StartAnchorLevel + step, Param::LevelRange,
                        "Start level");
      config_toggle<2>(this, Param::StartAnchorMode + step, "Start anchor mode",
                       {"Sample the source", "Track the source"});

      config_toggle<anchor_source_count>(this, Param::EndAnchorSource + step,
                                         "End anchor source",
                                         {"Level", "A", "B", "C", "Out"});
      config_level_knob(this, Param::EndAnchorLevel + step, Param::LevelRange,
                        "End level");
      config_toggle<2>(this, Param::EndAnchorMode + step, "End anchor mode",
                       {"Sample the source", "Track the source"}, 1);

      config_knob(this, Param::Duration + step, "Duration", "", Range{0.F, 1.F},
                  centered_rotation);

      config_curve_shape_switch(this, Param::Shape + step, "Shape");
      config_curvature_knob(this, Param::Curvature + step, "Curvature");

      show_inactive(step);
    }
  }

  ~Module() override = default;

  void process(ProcessArgs const & /*args*/) override {
    sequence_controller_.execute();
  }

  auto anchor_mode(AnchorType type, int step) const -> AnchorMode {
    auto const base = type == AnchorType::Start ? Param::StepStartAnchorMode
                                                : Param::StepEndAnchorMode;
    auto const selection = position_of(params[base + step]);
    return static_cast<AnchorMode>(selection);
  }

  auto anchor_level(AnchorType type, int step) const -> float {
    auto const base_knob_param = type == AnchorType::Start
                                     ? Param::StepStartAnchorLevel
                                     : Param::StepEndAnchorLevel;
    return scannable::level(
        params[base_knob_param + step], params[Param::LevelRange],
        params[Param::LevelMultiplier], inputs[Input::LevelAttenuationCV]);
  }

  auto anchor_source(AnchorType type, int step) const -> AnchorSource {
    auto const base = type == AnchorType::Start ? Param::StepStartAnchorSource
                                                : Param::StepEndAnchorSource;
    auto const selection = position_of(params[base + step]);
    return static_cast<AnchorSource>(selection);
  }

  auto curvature(int step) const -> float {
    return dhe::curvature(params[Param::StepCurvature + step]);
  }

  auto duration(int step) const -> float {
    return scannable::duration(
        params[Param::StepDuration + step], params[Param::DurationRange],
        params[Param::DurationMultiplier], inputs[Input::DurationMultiplierCV]);
  }

  auto in_a() const -> float { return voltage_at(inputs[Input::InA]); }

  auto in_b() const -> float { return voltage_at(inputs[Input::InB]); }

  auto in_c() const -> float { return voltage_at(inputs[Input::InC]); }

  auto output() const -> float { return voltage_at(outputs[Output::Out]); }

  void output(float voltage) { outputs[Output::Out].setVoltage(voltage); }

  void show_inactive(int step) { set_lights(step, 0.F, 0.F); }

  void show_progress(int step, float progress) {
    auto const completed_brightness = brightness_range.scale(progress);
    auto const remaining_brightness = 1.F - completed_brightness;
    set_lights(step, completed_brightness, remaining_brightness);
  }

  auto taper(int step) const -> sigmoid::Taper const & {
    auto const selection = position_of(params[Param::StepShape + step]);
    return sigmoid::tapers[selection];
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  using AnchorT = Anchor<Module>;
  using GeneratorT = Generator<Module, AnchorT>;
  using StepSelectorT = StepSelector<Module>;

  AnchorT end_anchor_{*this, AnchorType::End};
  AnchorT start_anchor_{*this, AnchorType::Start};
  GeneratorT generator_{*this, start_anchor_, end_anchor_};
  StepSelectorT step_selector_{*this, N};

  using StepControllerT = StepController<GeneratorT>;
  using SequenceControllerT =
      SequenceController<Module, StepSelectorT, StepControllerT>;

  StepControllerT step_controller_{generator_};
  SequenceControllerT sequence_controller_{*this, step_selector_,
                                           step_controller_};

  void set_lights(int step, float completed_brightness,
                  float remaining_brightness) {
    auto const completed_light = Light::Progress + step + step;
    auto const remaining_light = completed_light + 1;
    lights[completed_light].setBrightness(completed_brightness);
    lights[remaining_light].setBrightness(remaining_brightness);
  }
};
} // namespace scannable

} // namespace dhe
