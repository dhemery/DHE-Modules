#pragma once

#include "./upstage-controls.h"
#include "./upstage-engine.h"
#include "params/common-config.h"
#include "params/level-config.h"

#include "rack.hpp"

namespace dhe {
namespace stage {
class UpstageModule : public rack::engine::Module {
  using Controls = UpstageControls;

public:
  UpstageModule() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

    config_level_knob(this, Controls::LevelKnob, Controls::LevelRangeSwitch);
    config_level_range_switch(this, Controls::LevelRangeSwitch);
    configInput(Controls::LevelCvInput, "Level CV");

    configInput(Controls::TriggerInput, "Trigger");
    config_button(this, Controls::TriggerButton, "Trigger");

    configInput(Controls::WaitInput, "Wait");
    config_button(this, Controls::WaitButton, "Wait");

    configOutput(Controls::TriggerOutput, "Trigger");
    configOutput(Controls::EnvelopeOutput, "Stage");
  }

  void process(ProcessArgs const & /*args*/) override { machine_.process(); }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  Controls controls_{inputs, params, outputs};
  UpstageEngine<Controls> machine_{controls_};
};
} // namespace stage
} // namespace dhe
