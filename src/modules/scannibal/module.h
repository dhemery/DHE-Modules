#pragma once

#include "anchor.h"
#include "control-ids.h"
#include "controller.h"
#include "generator.h"

#include "components/cxmath.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "params/presets.h"
#include "signals/basic.h"
#include "signals/curvature.h"
#include "signals/durations.h"
#include "signals/gain.h"
#include "signals/step-selection.h"
#include "signals/voltages.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {
namespace scannibal {

static auto constexpr brightness_range = Range{0.F, 1.F};

template <int N> class Module : public rack::engine::Module {
  using InputId = InputIds<N>;
  using LightId = LightIds<N>;
  using OutputId = OutputIds;
  using ParamId = ParamIds<N>;

public:
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count, LightId::Count);
    Knob::config<SelectionLength<N>>(this, ParamId::SelectionLength, "Steps",
                                     N);

    configInput(InputId::InA, "A");
    configInput(InputId::InB, "B");
    configInput(InputId::InC, "C");
    configInput(InputId::Phase, "Phase");

    configOutput(OutputId::StepNumber, "Step number");
    configOutput(OutputId::StepPhase, "Step phase");
    configOutput(OutputId::Out, "Scanner");

    auto level_knobs = std::vector<MappedKnobQuantity<Voltage> *>{};

    for (auto step = 0; step < N; step++) {
      auto const step_name = "Step " + std::to_string(step + 1) + " ";
      configLight(LightId::Progress + step + step, step_name + "phase");
      Switch::config<AnchorSources>(
          this, ParamId::StepPhase0AnchorSource + step,
          step_name + "phase 0 anchor source", AnchorSource::Out);
      auto *phase_0_level_knob =
          Knob::config<Voltage>(this, ParamId::StepPhase0AnchorLevel + step,
                                step_name + "phase 0 level");
      level_knobs.push_back(phase_0_level_knob);
      configInput(InputId::StepPhase0AnchorLevelCv + step,
                  step_name + "phase 0 level CV");
      Switch::config<AnchorModes>(this, ParamId::StepPhase0AnchorMode + step,
                                  step_name + "phase 0 anchor mode",
                                  AnchorMode::Sample);

      Switch::config<AnchorSources>(
          this, ParamId::StepPhase1AnchorSource + step,
          step_name + "phase 1 anchor source", AnchorSource::Level);
      auto *phase_1_level_knob =
          Knob::config<Voltage>(this, ParamId::StepPhase1AnchorLevel + step,
                                step_name + "phase 1 level");
      level_knobs.push_back(phase_1_level_knob);
      configInput(InputId::StepPhase1AnchorLevelCv + step,
                  step_name + "phase 1 level CV");
      Switch::config<AnchorModes>(this, ParamId::StepPhase1AnchorMode + step,
                                  step_name + "phase 1 anchor mode",
                                  AnchorMode::Track);

      Switch::config<Shapes>(this, ParamId::StepShape + step,
                             step_name + "shape", Shape::Id::J);
      Knob::config<Curvature>(this, ParamId::StepCurvature + step,
                              step_name + "curvature");
      configInput(InputId::StepCurvatureCv + step, step_name + "curvature CV");

      Knob::config<Gain>(this, ParamId::StepDuration + step,
                         step_name + "relative duration");
      configInput(InputId::StepDurationCv + step,
                  step_name + "relative duration CV");
    }

    auto select_level_range = [level_knobs](VoltageRangeId id) {
      for (auto *knob : level_knobs) {
        knob->mapper().select_range(id);
      }
    };
    Switch::config<VoltageRanges>(this, ParamId::LevelRange, "Level range",
                                  VoltageRangeId::Unipolar)
        ->on_change(select_level_range);
  }

  ~Module() override = default;

  void process(ProcessArgs const & /*args*/) override { controller_.execute(); }

  auto anchor_mode(AnchorType type, int step) const -> AnchorMode {
    auto const base = type == AnchorType::Phase0
                          ? ParamId::StepPhase0AnchorMode
                          : ParamId::StepPhase1AnchorMode;
    return value_of<AnchorMode>(params[base + step]);
  }

  auto anchor_level(AnchorType type, int step) const -> float {
    auto const base_knob_param = type == AnchorType::Phase0
                                     ? ParamId::StepPhase0AnchorLevel
                                     : ParamId::StepPhase1AnchorLevel;
    auto const base_cv_input = type == AnchorType::Phase0
                                   ? InputId::StepPhase0AnchorLevelCv
                                   : InputId::StepPhase1AnchorLevelCv;
    auto const rotation = rotation_of(params[base_knob_param + step],
                                      inputs[base_cv_input + step]);
    auto const range_id = value_of<VoltageRangeId>(params[ParamId::LevelRange]);
    return Voltage::scale(rotation, range_id);
  }

  auto anchor_source(AnchorType type, int step) const -> AnchorSource {
    auto const base = type == AnchorType::Phase0
                          ? ParamId::StepPhase0AnchorSource
                          : ParamId::StepPhase1AnchorSource;
    return value_of<AnchorSource>(params[base + step]);
  }

  auto curvature(int step) const -> float {
    auto const rotation = rotation_of(params[ParamId::StepCurvature + step],
                                      inputs[InputId::StepCurvatureCv + step]);
    return Curvature::scale(Rotation::clamp(rotation));
  }

  auto duration(int step) const -> float {
    auto const rotation = rotation_of(params[ParamId::StepDuration + step],
                                      inputs[InputId::StepDurationCv + step]);
    return cx::max(rotation, 0.F);
  }

  void exit_step(int step) { set_lights(step, 0.F, 0.F); }

  auto in_a() const -> float { return voltage_at(inputs[InputId::InA]); }

  auto in_b() const -> float { return voltage_at(inputs[InputId::InB]); }

  auto in_c() const -> float { return voltage_at(inputs[InputId::InC]); }

  auto length() const -> float {
    return value_of(params[ParamId::SelectionLength]);
  }

  auto phase() const -> float { return voltage_at(inputs[InputId::Phase]); }

  auto output() const -> float { return voltage_at(outputs[OutputId::Out]); }

  void output(float voltage) { outputs[OutputId::Out].setVoltage(voltage); }

  void show_position(int step, float phase) {
    auto const completed_brightness = brightness_range.scale(phase);
    auto const remaining_brightness = 1.F - completed_brightness;
    set_lights(step, completed_brightness, remaining_brightness);
    outputs[OutputId::StepNumber].setVoltage(static_cast<float>(step + 1) *
                                             10.F / static_cast<float>(N));
    outputs[OutputId::StepPhase].setVoltage(phase * 10.F);
  }

  auto shape(int step) const -> Shape::Id {
    return value_of<Shape::Id>(params[ParamId::StepShape + step]);
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
    auto const completed_light = LightId::Progress + step + step;
    auto const remaining_light = completed_light + 1;
    lights[completed_light].setBrightness(completed_brightness);
    lights[remaining_light].setBrightness(remaining_brightness);
  }
};
} // namespace scannibal

} // namespace dhe
