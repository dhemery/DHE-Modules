#include "init.h"
#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace scannibal {
struct Scannibal4 {
  static auto constexpr panel_file = "scannibal-4";
  static auto constexpr step_count = 4;
};

struct Scannibal8 {
  static auto constexpr panel_file = "scannibal-8";
  static auto constexpr step_count = 8;
};

struct Scannibal16 {
  static auto constexpr panel_file = "scannibal-16";
  static auto constexpr step_count = 16;
};

void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(
      rack::createModel<Module<4>, Panel<Scannibal4>>("Scannibal4"));
  plugin->addModel(
      rack::createModel<Module<8>, Panel<Scannibal8>>("Scannibal8"));
  plugin->addModel(
      rack::createModel<Module<16>, Panel<Scannibal16>>("Scannibal16"));
}
} // namespace scannibal
} // namespace dhe
