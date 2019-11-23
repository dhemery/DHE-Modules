#pragma once

#include "UpstageControls.h"
#include "config/CommonConfig.h"
#include "config/LevelConfig.h"
#include "controls/CommonInputs.h"
#include "controls/LevelInputs.h"

#include <engine/Module.hpp>

namespace dhe {
namespace stage {

  template <typename Controls> class Upstage {
  public:
    Upstage(Controls &controls) : controls{controls} {}

    void process() {
      auto isTriggered = controls.isTriggered() && !controls.isWaiting();
      controls.sendTrigger(isTriggered);
      controls.sendEnvelope(controls.level());
    }

  private:
    Controls &controls;
  };
} // namespace stage
} // namespace dhe
