#include "init.h"
#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace tapers {
void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Module, Panel>("Tapers"));
}
} // namespace tapers
} // namespace dhe
