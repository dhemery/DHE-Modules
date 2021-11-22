#include "init.h"
#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace curve_sequencer {
struct Steps4 {
  static auto constexpr panel_file = "curve-sequencer-4";
  static auto constexpr step_count = 4;
};

struct Steps8 {
  static auto constexpr panel_file = "curve-sequencer-8";
  static auto constexpr step_count = 8;
};

struct Steps16 {
  static auto constexpr panel_file = "curve-sequencer-16";
  static auto constexpr step_count = 16;
};

void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(
      rack::createModel<Module<4>, Panel<Steps4>>("CurveSequencer4"));
  plugin->addModel(
      rack::createModel<Module<8>, Panel<Steps8>>("CurveSequencer8"));
  plugin->addModel(
      rack::createModel<Module<16>, Panel<Steps16>>("CurveSequencer16"));
}
} // namespace curve_sequencer
} // namespace dhe
