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

    configInput(Controls::EnvelopeInput, "Stage");
    configOutput(Controls::EnvelopeOutput, "Stage");

    config_level_knob(this, Controls::LevelKnob, Controls::LevelRangeSwitch);
    config_level_range_switch(this, Controls::LevelRangeSwitch);
    configInput(Controls::LevelCvInput, "Level CV");

    config_curvature_knob(this, Controls::CurveKnob);
    config_curve_shape_switch(this, Controls::ShapeSwitch);
    configInput(Controls::CurveCvInput, "Curvature CV");

    config_duration_knob(this, Controls::DurationKnob,
                         Controls::DurationRangeSwitch);
    config_duration_range_switch(this, Controls::DurationRangeSwitch);
    configInput(Controls::DurationCvInput, "Duration CV");

    configInput(Controls::DeferInput, "Defer");
    config_button(this, Controls::DeferButton, "Defer");
    configInput(Controls::TriggerInput, "Trigger");
    config_button(this, Controls::TriggerButton, "Trigger");

    configOutput(Controls::ActiveOutput, "Is active");
    config_button(this, Controls::ActiveButton, "Is active");
    configOutput(Controls::EocOutput, "End of stage");
    config_button(this, Controls::EocButton, "End of stage");
  }

  void process(ProcessArgs const &args) override {
    machine_.process(args.sampleTime);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
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
