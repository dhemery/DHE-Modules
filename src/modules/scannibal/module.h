#pragma once

#include "anchor.h"
#include "components/cxmath.h"
#include "config/common-config.h"
#include "config/curvature-config.h"
#include "config/duration-config.h"
#include "config/level-config.h"
#include "control-ids.h"
#include "controller.h"
#include "controls/curvature-inputs.h"
#include "controls/duration-inputs.h"
#include "controls/level-inputs.h"
#include "generator.h"

#include <engine/Module.hpp>
#include <jansson.h>

namespace dhe {
namespace scannibal {

static auto constexpr brightness_range = Range{0.F, 1.F};

template <typename P>
static inline auto level(P const &level_knob, P const &range_switch) -> float {
  auto const range = level_range(range_switch);
  auto const rotation = dhe::rotation_of(level_knob);
  return range.scale(rotation);
}

template <int N> class Module : public rack::engine::Module {
  using Input = InputIds<N>;
  using Light = LightIds<N>;
  using Output = OutputIds;
  using Param = ParamIds<N>;

public:
  Module() {
    config(Param::Count, Input::Count, Output::Count, Light::Count);

    auto step_knob = configParam(Param::Length, 1.F, N, N, "Steps", "");
    step_knob->snapEnabled = true;

    config_level_range_switch(this, Param::LevelRange);

    configInput(Input::InA, "A");
    configInput(Input::InB, "B");
    configInput(Input::InC, "C");
    configInput(Input::Phase, "Phase");

    configOutput(Output::StepNumber, "Step number");
    configOutput(Output::StepPhase, "Step phase");
    configOutput(Output::Out, "Scanner");

    for (auto step = 0; step < N; step++) {
      auto const step_name = "Step " + std::to_string(step + 1) + " ";
      configLight(Light::Progress + step + step, step_name + "phase");
      config_toggle<anchor_source_count>(this, Param::Phase0AnchorSource + step,
                                         step_name + "phase 0 anchor source",
                                         {"Level", "A", "B", "C", "Out"}, 4);
      config_level_knob(this, Param::Phase0AnchorLevel + step,
                        Param::LevelRange, step_name + "phase 0 level");
      configInput(Input::Phase0AnchorLevelCV + step,
                  step_name + "phase 0 level CV");
      config_toggle<2>(this, Param::Phase0AnchorMode + step,
                       step_name + "phase 0 anchor mode",
                       {"Sample the source", "Track the source"});

      config_toggle<anchor_source_count>(this, Param::Phase1AnchorSource + step,
                                         step_name + "phase 1 anchor source",
                                         {"Level", "A", "B", "C", "Out"});
      config_level_knob(this, Param::Phase1AnchorLevel + step,
                        Param::LevelRange, step_name + "phase 1 level");
      configInput(Input::Phase1AnchorLevelCV + step,
                  step_name + "phase 1 level CV");
      config_toggle<2>(this, Param::Phase1AnchorMode + step,
                       step_name + "phase 1 anchor mode",
                       {"Sample the source", "Track the source"}, 1);

      config_curve_shape_switch(this, Param::Shape + step, step_name + "shape");
      config_curvature_knob(this, Param::Curvature + step,
                            step_name + "curvature");
      configInput(Input::CurvatureCV + step, step_name + "curvature CV");

      config_knob(this, Param::Duration + step, step_name + "relative duration",
                  "", Range{0.F, 2.F}, centered_rotation);
      configInput(Input::DurationCV + step, step_name + "relative duration CV");
    }
  }

  ~Module() override = default;

  void process(ProcessArgs const & /*args*/) override { controller_.execute(); }

  auto anchor_mode(AnchorType type, int step) const -> AnchorMode {
    auto const base = type == AnchorType::Phase0 ? Param::Phase0AnchorMode
                                                 : Param::Phase1AnchorMode;
    auto const selection = position_of(params[base + step]);
    return static_cast<AnchorMode>(selection);
  }

  auto anchor_level(AnchorType type, int step) const -> float {
    auto const base_knob_param = type == AnchorType::Phase0
                                     ? Param::Phase0AnchorLevel
                                     : Param::Phase1AnchorLevel;
    return scannibal::level(params[base_knob_param + step],
                            params[Param::LevelRange]);
  }

  auto anchor_source(AnchorType type, int step) const -> AnchorSource {
    auto const base = type == AnchorType::Phase0 ? Param::Phase0AnchorSource
                                                 : Param::Phase1AnchorSource;
    auto const selection = position_of(params[base + step]);
    return static_cast<AnchorSource>(selection);
  }

  auto curvature(int step) const -> float {
    return dhe::curvature(params[Param::Curvature + step],
                          inputs[Input::CurvatureCV + step]);
  }

  auto duration(int step) const -> float {
    return cx::clamp(dhe::rotation(params[Param::Duration + step],
                                   inputs[Input::DurationCV + step]),
                     0.F, 1.F);
  }

  void exit_step(int step) { set_lights(step, 0.F, 0.F); }

  auto in_a() const -> float { return voltage_at(inputs[Input::InA]); }

  auto in_b() const -> float { return voltage_at(inputs[Input::InB]); }

  auto in_c() const -> float { return voltage_at(inputs[Input::InC]); }

  auto length() const -> float { return value_of(params[Param::Length]); }

  auto phase() const -> float { return voltage_at(inputs[Input::Phase]); }

  auto output() const -> float { return voltage_at(outputs[Output::Out]); }

  void output(float voltage) { outputs[Output::Out].setVoltage(voltage); }

  void show_position(int step, float phase) {
    auto const completed_brightness = brightness_range.scale(phase);
    auto const remaining_brightness = 1.F - completed_brightness;
    set_lights(step, completed_brightness, remaining_brightness);
    outputs[Output::StepNumber].setVoltage(static_cast<float>(step + 1) * 10.F /
                                           static_cast<float>(N));
    outputs[Output::StepPhase].setVoltage(phase * 10.F);
  }

  auto taper(int step) const -> sigmoid::Taper const & {
    auto const selection = position_of(params[Param::Shape + step]);
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
  using ControllerT = Controller<Module, GeneratorT, N>;

  AnchorT phase_0_anchor_{*this, AnchorType::Phase0};
  AnchorT phase_1_anchor_{*this, AnchorType::Phase1};
  GeneratorT generator_{*this, phase_0_anchor_, phase_1_anchor_};
  ControllerT controller_{*this, generator_};

  void set_lights(int step, float completed_brightness,
                  float remaining_brightness) {
    auto const completed_light = Light::Progress + step + step;
    auto const remaining_light = completed_light + 1;
    lights[completed_light].setBrightness(completed_brightness);
    lights[remaining_light].setBrightness(remaining_brightness);
  }
};
} // namespace scannibal

} // namespace dhe
