#pragma once

#include "BoosterStageControls.h"
#include "StageEngine.h"
#include "config/CurvatureConfig.h"
#include "config/DurationConfig.h"
#include "config/LevelConfig.h"

#include <engine/Module.hpp>

namespace dhe {
namespace stage {
  class BoosterStageModule : public rack::engine::Module {
    using Controls = BoosterStageControls;

  public:
    BoosterStageModule() : rack::engine::Module{} {
      config(Controls::ParameterCount, Controls::InputCount, Controls::OutputCount);

      configDurationKnob(this, Controls::DurationKnob, Controls::DurationRangeSwitch);
      configDurationRangeSwitch(this, Controls::DurationRangeSwitch);

      configLevelKnob(this, Controls::LevelKnob, Controls::LevelRangeSwitch);
      configLevelRangeSwitch(this, Controls::LevelRangeSwitch);

      configCurvatureKnob(this, Controls::CurveKnob);
      configCurveShapeSwitch(this, Controls::ShapeSwitch);

      configButton(this, Controls::DeferButton, "DEFER", {"From input", "High"}, 0);
      configButton(this, Controls::TriggerButton, "TRIG", {"From input", "High"}, 0);
      configButton(this, Controls::ActiveButton, "ACTIVE", {"Generated", "High"}, 0);
      configButton(this, Controls::EocButton, "EOC", {"Generated", "High"}, 0);
    }

    void process(const ProcessArgs &args) override { machine.process(args.sampleTime); }

  private:
    Controls controls{inputs, params, outputs};
    StageEngine<Controls> machine{controls};
  };
} // namespace stage
} // namespace dhe
