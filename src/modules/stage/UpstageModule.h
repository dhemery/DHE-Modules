#pragma once

#include "UpstageControls.h"
#include "UpstageEngine.h"
#include "config/CommonConfig.h"
#include "config/LevelConfig.h"

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

    config_button(this, Controls::TriggerButton, "TRIG", {"From input", "High"},
                  0);
    config_button(this, Controls::WaitButton, "WAIT", {"From input", "High"},
                  0);
  }

  void process(ProcessArgs const & /*args*/) override { machine.process(); }

private:
  Controls controls{inputs, params, outputs};
  UpstageEngine<Controls> machine{controls};
};
} // namespace stage
} // namespace dhe
