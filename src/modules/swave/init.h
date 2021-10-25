#pragma once
#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace swave {
static inline void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Swave, SwavePanel>("Swave"));
}
} // namespace swave
} // namespace dhe
