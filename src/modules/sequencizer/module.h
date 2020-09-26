#pragma once

#include "advancement.h"
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

namespace dhe {
namespace sequencizer {

// Skew the progress::brightness ratio so that the "remaining" light stays
// fully lit for a little while during early progress, and the "completed"
// light reaches fully lit a little while before progress is complete.
static auto constexpr brightness_skew = 0.7F;
static auto constexpr brightness_range =
    Range{-brightness_skew, 1.F + brightness_skew};

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
  return cx::max(1e-3F, scaled_duration);
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
    config_button(this, Param::Run, "Run", {"From input", "Yes"}, 1);
    config_button(this, Param::Gate, "Gate", {"From input", "High"}, 0);
    config_button(this, Param::Loop, "Loop", {"From input", "Yes"}, 0);
    config_button(this, Param::Reset, "Reset", {"From input", "High"}, 0);

    configParam(Param::SelectionStart, 0.F, N - 1, 0.F, "Start step", "", 0.F,
                1.F, 1.F);
    configParam(Param::SelectionLength, 1.F, N, N, "Sequence length", " steps");

    config_attenuator(this, Param::LevelMultiplier, "Level multiplier");
    config_level_range_switch(this, Param::LevelRange);

    config_gain(this, Param::DurationMultiplier, "Duration multiplier");
    config_duration_range_switch(this, Param::DurationRange);

    for (auto step = 0; step < N; step++) {
      config_toggle<trigger_mode_count>(this, Param::StepTriggerMode + step,
                                        "Trigger mode",
                                        trigger_mode_descriptions, 0);
      config_toggle<2>(
          this, Param::StepInterruptMode + step, "Interrupt",
          {"Ignore triggers while generating", "Interrupt if triggered"});
      config_toggle<2>(this, Param::StepSustainMode + step, "Sustain",
                       {"No sustain", "Sustain until triggered"}, 0);

      config_toggle<anchor_source_count>(
          this, Param::StepStartAnchorSource + step, "Start anchor source",
          {"Level", "A", "B", "C", "Out"}, 4);
      config_level_knob(this, Param::StepStartAnchorLevel + step,
                        Param::LevelRange, "Start level");
      config_toggle<2>(this, Param::StepStartAnchorMode + step,
                       "Start anchor mode",
                       {"Sample the source", "Track the source"});

      config_toggle<anchor_source_count>(
          this, Param::StepEndAnchorSource + step, "End anchor source",
          {"Level", "A", "B", "C", "Out"});
      config_level_knob(this, Param::StepEndAnchorLevel + step,
                        Param::LevelRange, "End level");
      config_toggle<2>(this, Param::StepEndAnchorMode + step, "End anchor mode",
                       {"Sample the source", "Track the source"}, 1);

      config_curve_shape_switch(this, Param::StepShape + step, "Shape");
      config_curvature_knob(this, Param::StepCurvature + step, "Curvature");
      config_duration_knob(this, Param::StepDuration + step,
                           Param::DurationRange, "Duration");
      config_button(this, Param::StepEnabled + step, "Enabled", {"No", "Yes"},
                    1);

      show_inactive(step);
    }
  }

  ~Module() override = default;

  void process(ProcessArgs const &args) override {
    sequence_controller_.execute(args.sampleTime);
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
    return sequencizer::level(
        params[base_knob_param + step], params[Param::LevelRange],
        params[Param::LevelMultiplier], inputs[Input::LevelAttenuationCV]);
  }

  auto anchor_source(AnchorType type, int step) const -> AnchorSource {
    auto const base = type == AnchorType::Start ? Param::StepStartAnchorSource
                                                : Param::StepEndAnchorSource;
    auto const selection = position_of(params[base + step]);
    return static_cast<AnchorSource>(selection);
  }

  auto completion_mode(int step) const -> SustainMode {
    auto const selection = position_of(params[Param::StepSustainMode + step]);
    return static_cast<SustainMode>(selection);
  }

  auto curvature(int step) const -> float {
    return dhe::curvature(params[Param::StepCurvature + step]);
  }

  auto duration(int step) const -> float {
    return sequencizer::duration(
        params[Param::StepDuration + step], params[Param::DurationRange],
        params[Param::DurationMultiplier], inputs[Input::DurationMultiplierCV]);
  }

  auto gate() const -> bool {
    return is_high(inputs[Input::Gate]) || is_pressed(params[Param::Gate]);
  }

  auto in_a() const -> float { return voltage_at(inputs[Input::InA]); }

  auto in_b() const -> float { return voltage_at(inputs[Input::InB]); }

  auto in_c() const -> float { return voltage_at(inputs[Input::InC]); }

  auto interrupt_mode(int step) const -> InterruptMode {
    auto const selection = position_of(params[Param::StepInterruptMode + step]);
    return static_cast<InterruptMode>(selection);
  }

  auto is_enabled(int step) const -> bool {
    return is_pressed(params[Param::StepEnabled + step]);
  }

  auto is_looping() const -> bool {
    return is_pressed(params[Param::Loop]) || is_high(inputs[Input::Loop]);
  }

  auto is_reset() const -> bool {
    return is_high(inputs[Input::Reset]) || is_pressed(params[Param::Reset]);
  }

  auto is_running() const -> bool {
    return is_pressed(params[Param::Run]) || is_high(inputs[Input::Run]);
  }

  auto output() const -> float { return voltage_at(outputs[Output::Out]); }

  void output(float voltage) { outputs[Output::Out].setVoltage(voltage); }

  auto selection_start() const -> int {
    return static_cast<int>(rotation_of(params[Param::SelectionStart]));
  }

  auto selection_length() const -> int {
    return static_cast<int>(rotation_of(params[Param::SelectionLength]));
  }

  auto trigger_mode(int step) const -> TriggerMode {
    auto const selection = position_of(params[Param::StepTriggerMode + step]);
    return static_cast<TriggerMode>(selection);
  }

  void show_curving(bool curving) {
    outputs[Output::IsCurving].setVoltage(curving ? 10.F : 0.F);
  }

  void show_inactive(int step) { set_lights(step, 0.F, 0.F); }

  void show_progress(int step, float progress) {
    auto const completed_brightness = brightness_range.scale(progress);
    auto const remaining_brightness = 1.F - completed_brightness;
    set_lights(step, completed_brightness, remaining_brightness);
  }

  void show_sequence_event(bool event) {
    outputs[Output::SequenceEventPulse].setVoltage(event ? 10.F : 0.F);
  }

  void show_step_event(bool event) {
    outputs[Output::StepEventPulse].setVoltage(event ? 10.F : 0.F);
  }

  void show_step_status(int step, StepStatus status) {
    outputs[Output::StepNumber].setVoltage(static_cast<float>(step + 1) * 10.F /
                                           static_cast<float>(N));
    switch (status) {
    case StepStatus::Generating:
      outputs[Output::IsCurving].setVoltage(10.F);
      outputs[Output::IsSustaining].setVoltage(0.F);
      break;
    case StepStatus::Sustaining:
      outputs[Output::IsCurving].setVoltage(0.F);
      outputs[Output::IsSustaining].setVoltage(10.F);
      break;
    default:
      outputs[Output::IsCurving].setVoltage(0.F);
      outputs[Output::IsSustaining].setVoltage(0.F);
    }
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
  using InterrupterT = Interrupter<Module>;
  using StepSelectorT = StepSelector<Module>;
  using SustainerT = Sustainer<Module>;

  AnchorT end_anchor_{*this, AnchorType::End};
  AnchorT start_anchor_{*this, AnchorType::Start};
  GeneratorT generator_{*this, start_anchor_, end_anchor_};
  InterrupterT interrupter_{*this};
  StepSelectorT step_selector_{*this, N};
  SustainerT sustainer_{*this};

  using StepControllerT = StepController<InterrupterT, GeneratorT, SustainerT>;
  using SequenceControllerT =
      SequenceController<Module, StepSelectorT, StepControllerT>;

  StepControllerT step_controller_{interrupter_, generator_, sustainer_};
  SequenceControllerT sequence_controller_{*this, step_selector_,
                                           step_controller_};

  void set_lights(int step, float completed_brightness,
                  float remaining_brightness) {
    auto const completed_light = Light::StepProgress + step + step;
    auto const remaining_light = completed_light + 1;
    lights[completed_light].setBrightness(completed_brightness);
    lights[remaining_light].setBrightness(remaining_brightness);
  }
};
} // namespace sequencizer

} // namespace dhe
