#include "init.h"
#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace sequencizer {
struct Sequencizer4 {
  static auto constexpr panel_file = "sequencizer-4";
  static auto constexpr step_count = 4;
};

struct Sequencizer8 {
  static auto constexpr panel_file = "sequencizer-8";
  static auto constexpr step_count = 8;
};

struct Sequencizer16 {
  static auto constexpr panel_file = "sequencizer-16";
  static auto constexpr step_count = 16;
};

void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(
      rack::createModel<Module<4>, Panel<Sequencizer4>>("Sequencizer4"));
  plugin->addModel(
      rack::createModel<Module<8>, Panel<Sequencizer8>>("Sequencizer8"));
  plugin->addModel(
      rack::createModel<Module<16>, Panel<Sequencizer16>>("Sequencizer16"));
}
} // namespace sequencizer
} // namespace dhe
