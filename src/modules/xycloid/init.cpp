#include "init.h"
#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace xycloid {
void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Module, Panel>("Xycloid"));
}
} // namespace xycloid
} // namespace dhe
