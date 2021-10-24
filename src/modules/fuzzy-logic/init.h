#pragma once

#include "engines.h"
#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace fuzzy_logic {
static inline void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(
      rack::createModel<Module<HEngine>, Panel<HPanel>>("FuzzyLogicH"));
  plugin->addModel(
      rack::createModel<Module<ZEngine>, Panel<ZPanel>>("FuzzyLogicZ"));
}

} // namespace fuzzy_logic
} // namespace dhe
