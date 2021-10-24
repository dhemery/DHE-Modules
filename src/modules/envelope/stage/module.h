#pragma once

#include "controls.h"
#include "engine.h"

#include "modules/envelope/mode/defer.h"
#include "modules/envelope/mode/generate.h"
#include "modules/envelope/mode/input.h"
#include "modules/envelope/mode/level.h"
#include "params/curvature-config.h"
#include "params/duration-config.h"
#include "params/level-config.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace stage {
class Module : public rack::engine::Module {
  using Controls = StageControls;
  using DeferM = mode::DeferMode<Controls>;
  using GenerateM = mode::GenerateMode<Controls, PhaseTimer>;
  using InputM = mode::InputMode<Controls>;
  using LevelM = mode::LevelMode<Controls>;

public:
  Module() {
    config(StageControls::ParameterCount, StageControls::InputCount,
           StageControls::OutputCount);

    config_duration_knob(this, StageControls::DurationKnob,
                         medium_duration_range);
    config_level_knob(this, StageControls::LevelKnob, unipolar_signal_range);
    config_curvature_knob(this, StageControls::CurveKnob);

    configInput(Controls::EnvelopeInput, "Stage");
    configInput(Controls::DeferInput, "Defer");
    configInput(Controls::TriggerInput, "Trigger");
    configOutput(Controls::ActiveOutput, "Is active");
    configOutput(Controls::EocOutput, "End of stage");
    configOutput(Controls::EnvelopeOutput, "Stage");
  }

  void process(ProcessArgs const &args) override {
    machine_.process(args.sampleTime);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  Controls controls_{inputs, params, outputs};
  PhaseTimer timer_{};
  DeferM defer_mode_{controls_};
  InputM input_mode_{controls_};
  GenerateM generate_mode_{controls_, timer_};
  LevelM level_mode_{controls_};
  StageEngine<Controls, DeferM, InputM, GenerateM, LevelM> machine_{
      controls_, defer_mode_, input_mode_, generate_mode_, level_mode_};
};
} // namespace stage
} // namespace envelope
} // namespace dhe
