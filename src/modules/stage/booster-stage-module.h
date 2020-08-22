#pragma once

#include "booster-stage-controls.h"
#include "config/curvature-config.h"
#include "config/duration-config.h"
#include "config/level-config.h"
#include "defer-mode.h"
#include "generate-mode.h"
#include "input-mode.h"
#include "level-mode.h"
#include "stage-engine.h"

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
    machine_.process(args.sampleTime);
  }

private:
  Controls controls_{inputs, params, outputs};
  PhaseTimer timer_{};
  DeferMode<Controls> defer_mode_{controls_};
  InputMode<Controls> input_mode_{controls_};
  GenerateMode<Controls, PhaseTimer> generate_mode_{controls_, timer_};
  LevelMode<Controls> level_mode_{controls_};
  StageEngine<Controls, DeferMode<Controls>, InputMode<Controls>,
              GenerateMode<Controls, PhaseTimer>, LevelMode<Controls>>
      machine_{controls_, defer_mode_, input_mode_, generate_mode_,
               level_mode_};
};
} // namespace stage
} // namespace dhe
