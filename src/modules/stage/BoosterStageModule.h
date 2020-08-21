#pragma once

#include "BoosterStageControls.h"
#include "DeferMode.h"
#include "GenerateMode.h"
#include "InputMode.h"
#include "LevelMode.h"
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
  BoosterStageModule() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

    config_duration_knob(this, Controls::DurationKnob,
                         Controls::DurationRangeSwitch);
    config_duration_range_switch(this, Controls::DurationRangeSwitch);

    config_level_knob(this, Controls::LevelKnob, Controls::LevelRangeSwitch);
    config_level_range_switch(this, Controls::LevelRangeSwitch);

    config_curvature_knob(this, Controls::CurveKnob);
    config_curve_shape_switch(this, Controls::ShapeSwitch);

    config_button(this, Controls::DeferButton, "DEFER", {"From input", "High"},
                  0);
    config_button(this, Controls::TriggerButton, "TRIG", {"From input", "High"},
                  0);
    config_button(this, Controls::ActiveButton, "ACTIVE", {"Generated", "High"},
                  0);
    config_button(this, Controls::EocButton, "EOC", {"Generated", "High"}, 0);
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
