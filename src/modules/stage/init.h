#pragma once
#include "booster-stage-module.h"
#include "booster-stage-panel.h"
#include "hostage-module.h"
#include "hostage-panel.h"
#include "stage-module.h"
#include "stage-panel.h"
#include "upstage-module.h"
#include "upstage-panel.h"

#include "rack.hpp"

namespace dhe {
namespace stage {
static inline void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(
      rack::createModel<BoosterStageModule, BoosterStagePanel>("BoosterStage"));
  plugin->addModel(rack::createModel<HostageModule, HostagePanel>("Hostage"));
  plugin->addModel(rack::createModel<StageModule, StagePanel>("Stage"));
  plugin->addModel(rack::createModel<UpstageModule, UpstagePanel>("Upstage"));
}
} // namespace stage
} // namespace dhe
