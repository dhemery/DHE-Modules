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
    StageModule() : rack::engine::Module{} {
      config(StageControls::ParameterCount, StageControls::InputCount, StageControls::OutputCount);

      configDurationKnob(this, StageControls::DurationKnob, mediumDurationRange);
      configLevelKnob(this, StageControls::LevelKnob, unipolarSignalRange);
      configCurvatureKnob(this, StageControls::CurveKnob);
    }

    void process(const ProcessArgs &args) override { machine.process(args.sampleTime); }

  private:
    Controls controls{inputs, params, outputs};
    PhaseTimer timer{};
    DeferMode<Controls> deferMode{controls};
    InputMode<Controls> inputMode{controls};
    GenerateMode<Controls, PhaseTimer> generateMode{controls, timer};
    StageEngine<Controls, DeferMode<Controls>, InputMode<Controls>, GenerateMode<Controls, PhaseTimer>> machine{
        controls, deferMode, inputMode, generateMode};
  };
} // namespace stage
} // namespace dhe
