#include "init.h"
#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace gator {
void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Module, Panel>("Gator"));
}
} // namespace gator
} // namespace dhe
