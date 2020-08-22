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

    config_button(this, Controls::TriggerButton, "TRIG", {"From input", "High"},
                  0);
    config_button(this, Controls::WaitButton, "WAIT", {"From input", "High"},
                  0);
  }

  void process(ProcessArgs const & /*args*/) override { machine_.process(); }

private:
  Controls controls_{inputs, params, outputs};
  UpstageEngine<Controls> machine_{controls_};
};
} // namespace stage
} // namespace dhe
