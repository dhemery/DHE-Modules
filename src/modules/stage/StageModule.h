#pragma once

#include "DeferMode.h"
#include "GenerateMode.h"
#include "InputMode.h"
#include "StageControls.h"
#include "StageEngine.h"
#include "config/curvature-config.h"
#include "config/duration-config.h"
#include "config/level-config.h"

#include <engine/Module.hpp>

namespace dhe {
namespace stage {
class StageModule : public rack::engine::Module {
  using Controls = StageControls;

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
    machine.process(args.sampleTime);
  }

private:
  Controls controls{inputs, params, outputs};
  PhaseTimer timer{};
  DeferMode<Controls> deferMode{controls};
  InputMode<Controls> inputMode{controls};
  GenerateMode<Controls, PhaseTimer> generateMode{controls, timer};
  LevelMode<Controls> levelMode{controls};
  StageEngine<Controls, DeferMode<Controls>, InputMode<Controls>,
              GenerateMode<Controls, PhaseTimer>, LevelMode<Controls>>
      machine{controls, deferMode, inputMode, generateMode, levelMode};
};
} // namespace stage
} // namespace dhe
