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
      config(Controls::ParameterCount, Controls::InputCount, Controls::OutputCount);

      configLevelKnob(this, Controls::LevelKnob, Controls::LevelRangeSwitch);
      configLevelRangeSwitch(this, Controls::LevelRangeSwitch);

      configButton(this, Controls::TriggerButton, "TRIG", {"From input", "High"}, 0);
      configButton(this, Controls::WaitButton, "WAIT", {"From input", "High"}, 0);
    }

    void process(ProcessArgs const & /*args*/) override { machine.process(); }

  private:
    Controls controls{inputs, params, outputs};
    UpstageEngine<Controls> machine{controls};
  };
} // namespace stage
} // namespace dhe
