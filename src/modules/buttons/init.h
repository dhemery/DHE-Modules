#pragma once
#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace buttons {
static inline void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Module, Panel>("Buttons"));
}
} // namespace buttons
} // namespace dhe
