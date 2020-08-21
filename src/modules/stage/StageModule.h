#pragma once

#include "DeferMode.h"
#include "GenerateMode.h"
#include "InputMode.h"
#include "StageControls.h"
#include "StageEngine.h"
#include "config/CurvatureConfig.h"
#include "config/DurationConfig.h"
#include "config/LevelConfig.h"

#include <engine/Module.hpp>

namespace dhe {
namespace stage {
class StageModule : public rack::engine::Module {
  using Controls = StageControls;

public:
  StageModule() {
    config(StageControls::ParameterCount, StageControls::InputCount,
           StageControls::OutputCount);

    configDurationKnob(this, StageControls::DurationKnob, mediumDurationRange);
    configLevelKnob(this, StageControls::LevelKnob, unipolarSignalRange);
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
