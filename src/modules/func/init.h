#pragma once
#include "func-module.h"
#include "func1-panel.h"
#include "func6-panel.h"

#include "rack.hpp"

namespace dhe {
namespace func {
static inline void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<FuncModule<1>, Func1Panel>("Func"));
  plugin->addModel(
      rack::createModel<FuncModule<6>, Func6Panel>("Func6")); // NOLINT
}
} // namespace func
} // namespace dhe
