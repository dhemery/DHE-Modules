#pragma once

#include "fuzzy-logic-h-module.h"
#include "fuzzy-logic-z-module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace fuzzy_logic {
static inline void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(
      rack::createModel<FuzzyLogicHModule, Panel<HPanel>>("FuzzyLogicH"));
  plugin->addModel(
      rack::createModel<FuzzyLogicZModule, Panel<ZPanel>>("FuzzyLogicZ"));
}

} // namespace fuzzy_logic
} // namespace dhe
