#pragma once

#include "UpstageControls.h"
#include "config/common-config.h"
#include "config/level-config.h"
#include "controls/common-inputs.h"
#include "controls/level-inputs.h"

#include <engine/Module.hpp>

namespace dhe {
namespace stage {

template <typename Controls> class UpstageEngine {
public:
  UpstageEngine(Controls &controls) : controls{controls} {}

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
