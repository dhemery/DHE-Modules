#pragma once

#include "config/curvature-config.h"
#include "config/duration-config.h"
#include "config/level-config.h"
#include "defer-mode.h"
#include "generate-mode.h"
#include "input-mode.h"
#include "level-mode.h"
#include "stage-controls.h"
#include "stage-engine.h"

#include <engine/Module.hpp>

namespace dhe {
namespace stage {
class StageModule : public rack::engine::Module {
  using Controls = StageControls;
  using DeferMode = DeferMode<Controls>;
  using InputMode = InputMode<Controls>;
  using LevelMode = LevelMode<Controls>;
  using GenerateMode = GenerateMode<Controls, PhaseTimer>;

public:
  StageModule() {
    config(StageControls::ParameterCount, StageControls::InputCount,
           StageControls::OutputCount);

    config_duration_knob(this, StageControls::DurationKnob,
                         medium_duration_range);
    config_level_knob(this, StageControls::LevelKnob, unipolar_signal_range);
    config_curvature_knob(this, StageControls::CurveKnob);
  }

  void process(ProcessArgs const &args) override {
    machine_.process(args.sampleTime);
  }

private:
  Controls controls_{inputs, params, outputs};
  PhaseTimer timer_{};
  DeferMode defer_mode_{controls_};
  InputMode input_mode_{controls_};
  GenerateMode generate_mode_{controls_, timer_};
  LevelMode level_mode_{controls_};
  StageEngine<Controls, DeferMode, InputMode, GenerateMode, LevelMode> machine_{
      controls_, defer_mode_, input_mode_, generate_mode_, level_mode_};
};
} // namespace stage
} // namespace dhe
