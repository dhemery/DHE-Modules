#include "init.h"
#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace swave {
void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Module, Panel>("Swave"));
}
} // namespace swave
} // namespace dhe
