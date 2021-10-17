#pragma once
#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace cubic {
static inline void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Module, Panel>("Cubic"));
}
} // namespace cubic
} // namespace dhe
