#pragma once

#include "anchor.h"
#include "control-ids.h"
#include "controller.h"
#include "generator.h"
#include "params.h"

#include "components/cxmath.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "params/presets.h"
#include "signals/curvature.h"
#include "signals/durations.h"
#include "signals/step-selection.h"
#include "signals/voltages.h"

#include "rack.hpp"

namespace dhe {
namespace scannibal {

static auto constexpr brightness_range = Range{0.F, 1.F};

template <int N> class Module : public rack::engine::Module {
  using Input = InputIds<N>;
  using Light = LightIds<N>;
  using Output = OutputIds;
  using Param = ParamIds<N>;

public:
  Module() {
    config(Param::Count, Input::Count, Output::Count, Light::Count);
    IntKnob::config<SelectionLength<N>>(this, Param::SelectionLength, "Steps",
                                        N);

    configInput(Input::InA, "A");
    configInput(Input::InB, "B");
    configInput(Input::InC, "C");
    configInput(Input::Phase, "Phase");

    configOutput(Output::StepNumber, "Step number");
    configOutput(Output::StepPhase, "Step phase");
    configOutput(Output::Out, "Scanner");

    auto level_knobs = std::vector<MappedKnobQuantity<VoltageRanges> *>{};

    for (auto step = 0; step < N; step++) {
      auto const step_name = "Step " + std::to_string(step + 1) + " ";
      configLight(Light::Progress + step + step, step_name + "phase");
      Switch::config<AnchorSources>(this, Param::StepPhase0AnchorSource + step,
                                    step_name + "phase 0 anchor source",
                                    AnchorSource::Out);
      auto *phase_0_level_knob =
          Knob::config<VoltageRanges>(this, Param::StepPhase0AnchorLevel + step,
                                      step_name + "phase 0 level");
      level_knobs.push_back(phase_0_level_knob);
      configInput(Input::StepPhase0AnchorLevelCv + step,
                  step_name + "phase 0 level CV");
      Switch::config<AnchorModes>(this, Param::StepPhase0AnchorMode + step,
                                  step_name + "phase 0 anchor mode",
                                  AnchorMode::Sample);

      Switch::config<AnchorSources>(this, Param::StepPhase1AnchorSource + step,
                                    step_name + "phase 1 anchor source",
                                    AnchorSource::Level);
      auto *phase_1_level_knob =
          Knob::config<VoltageRanges>(this, Param::StepPhase1AnchorLevel + step,
                                      step_name + "phase 1 level");
      level_knobs.push_back(phase_1_level_knob);
      configInput(Input::StepPhase1AnchorLevelCv + step,
                  step_name + "phase 1 level CV");
      Switch::config<AnchorModes>(this, Param::StepPhase1AnchorMode + step,
                                  step_name + "phase 1 anchor mode",
                                  AnchorMode::Track);

      Switch::config<Shapes>(this, Param::StepShape + step, step_name + "shape",
                             Shape::J);
      Knob::config<Curvature>(this, Param::StepCurvature + step,
                              step_name + "curvature");
      configInput(Input::StepCurvatureCv + step, step_name + "curvature CV");

      Knob::config<Gain>(this, Param::StepDuration + step,
                         step_name + "relative duration");
      configInput(Input::StepDurationCv + step,
                  step_name + "relative duration CV");
    }

    auto select_level_range = [level_knobs](VoltageRangeId id) {
      for (auto *knob : level_knobs) {
        knob->mapper().select_range(id);
      }
    };
    Switch::config<VoltageRanges>(this, Param::LevelRange, "Level select",
                                  VoltageRangeId::Unipolar)
        ->on_change(select_level_range);
  }

  ~Module() override = default;

  void process(ProcessArgs const & /*args*/) override { controller_.execute(); }

  auto anchor_mode(AnchorType type, int step) const -> AnchorMode {
    auto const base = type == AnchorType::Phase0 ? Param::StepPhase0AnchorMode
                                                 : Param::StepPhase1AnchorMode;
    return value_of<AnchorMode>(params[base + step]);
  }

  auto anchor_level(AnchorType type, int step) const -> float {
    auto const base_knob_param = type == AnchorType::Phase0
                                     ? Param::StepPhase0AnchorLevel
                                     : Param::StepPhase1AnchorLevel;
    auto const base_cv_input = type == AnchorType::Phase0
                                   ? Input::StepPhase0AnchorLevelCv
                                   : Input::StepPhase1AnchorLevelCv;
    auto const rotation = rotation_of(params[base_knob_param + step],
                                      inputs[base_cv_input + step]);
    auto const range_id = value_of<VoltageRangeId>(params[Param::LevelRange]);
    return VoltageRanges::volts(rotation, range_id);
  }

  auto anchor_source(AnchorType type, int step) const -> AnchorSource {
    auto const base = type == AnchorType::Phase0
                          ? Param::StepPhase0AnchorSource
                          : Param::StepPhase1AnchorSource;
    return value_of<AnchorSource>(params[base + step]);
  }

  auto curvature(int step) const -> float {
    auto const rotation = rotation_of(params[Param::StepCurvature + step],
                                      inputs[Input::StepCurvatureCv + step]);
    return Curvature::curvature(rotation);
  }

  auto duration(int step) const -> float {
    auto const rotation = rotation_of(params[Param::StepDuration + step],
                                      inputs[Input::StepDurationCv + step]);
    return cx::clamp(rotation, 0.F, 1.F);
  }

  void exit_step(int step) { set_lights(step, 0.F, 0.F); }

  auto in_a() const -> float { return voltage_at(inputs[Input::InA]); }

  auto in_b() const -> float { return voltage_at(inputs[Input::InB]); }

  auto in_c() const -> float { return voltage_at(inputs[Input::InC]); }

  auto length() const -> float {
    return value_of(params[Param::SelectionLength]);
  }

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
    auto const shape =
        value_of<Shapes::Selection>(params[Param::StepShape + step]);
    return Shapes::taper(shape);
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
