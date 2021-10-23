#pragma once
#include "swave-panel.h"
#include "swave.h"

#include "rack.hpp"

namespace dhe {
namespace swave {
static inline void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Swave, SwavePanel>("Swave"));
}
} // namespace swave
} // namespace dhe
