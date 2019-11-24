#pragma once

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
    StageControls controls{inputs, params, outputs};
    PhaseTimer timer{};
    DeferMode<Controls> deferMode{controls};
    GenerateMode<Controls, PhaseTimer> generateMode{controls, timer};
    StageEngine<Controls, DeferMode<Controls>, GenerateMode<Controls, PhaseTimer>> machine{controls, deferMode,
                                                                                           generateMode};
  };
} // namespace stage
} // namespace dhe
