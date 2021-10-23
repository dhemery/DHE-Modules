#pragma once
#include "tapers-panel.h"
#include "tapers.h"

#include "rack.hpp"

namespace dhe {
namespace tapers {
static inline void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Tapers, TapersPanel>("Tapers"));
}
} // namespace tapers
} // namespace dhe
