#pragma once
#include "ranger-panel.h"
#include "ranger.h"

#include "rack.hpp"

namespace dhe {
namespace ranger {
static inline void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Ranger, RangerPanel>("Ranger"));
}
} // namespace ranger
} // namespace dhe
