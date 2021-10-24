#pragma once
#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace gator {
static inline void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Module, Panel>("Gator"));
}
} // namespace gator
} // namespace dhe
