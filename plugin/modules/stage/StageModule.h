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
    StageEngine<StageControls> machine{controls};
  };
} // namespace stage
} // namespace dhe
