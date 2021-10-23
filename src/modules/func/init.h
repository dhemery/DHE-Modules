#pragma once
#include "func1-panel.h"
#include "func6-panel.h"
#include "module.h"

#include "rack.hpp"

namespace dhe {
namespace func {
static inline void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Module<1>, Func1Panel>("Func"));
  plugin->addModel(rack::createModel<Module<6>, Func6Panel>("Func6")); // NOLINT
}
} // namespace func
} // namespace dhe
