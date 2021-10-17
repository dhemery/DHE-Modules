#pragma once
#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace curve_sequencer {
static inline void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Module<4>, Panel<4>>("CurveSequencer4"));
  plugin->addModel(rack::createModel<Module<8>, Panel<8>>("CurveSequencer8"));
  plugin->addModel(
      rack::createModel<Module<16>, Panel<16>>("CurveSequencer16"));
}
} // namespace curve_sequencer
} // namespace dhe
