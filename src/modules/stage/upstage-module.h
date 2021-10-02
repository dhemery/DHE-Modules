#pragma once

#include "config/common-config.h"
#include "config/level-config.h"
#include "upstage-controls.h"
#include "upstage-engine.h"

#include <engine/Module.hpp>

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

    config_button(this, Controls::TriggerButton, "TRIG");
    config_button(this, Controls::WaitButton, "WAIT");
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
