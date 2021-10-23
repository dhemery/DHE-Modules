#pragma once
#include "gator-panel.h"
#include "gator.h"

#include "rack.hpp"

namespace dhe {
namespace gator {
static inline void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Gator, GatorPanel>("Gator"));
}
} // namespace gator
} // namespace dhe
