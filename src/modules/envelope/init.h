#pragma once
#include "booster-stage/module.h"
#include "booster-stage/panel.h"
#include "hostage/module.h"
#include "hostage/panel.h"
#include "stage/module.h"
#include "stage/panel.h"
#include "upstage/module.h"
#include "upstage/panel.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
static inline void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(
      rack::createModel<booster_stage::Module, booster_stage::Panel>(
          "BoosterStage"));
  plugin->addModel(
      rack::createModel<hostage::Module, hostage::Panel>("Hostage"));
  plugin->addModel(rack::createModel<stage::Module, stage::Panel>("Stage"));
  plugin->addModel(
      rack::createModel<upstage::Module, upstage::Panel>("Upstage"));
}
} // namespace envelope
} // namespace dhe
