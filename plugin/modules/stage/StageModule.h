#pragma once

#include "Stage.h"
#include "StageControls.h"
#include "config/CurvatureConfig.h"
#include "config/DurationConfig.h"
#include "config/LevelConfig.h"

#include <engine/Module.hpp>

namespace dhe {
namespace stage {
  class StageModule : public rack::engine::Module {
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
    Stage<StageControls> machine{controls};
  };
} // namespace stage
} // namespace dhe
