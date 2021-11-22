#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace blossom {
void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Module, Panel>("Blossom"));
}
} // namespace blossom
} // namespace dhe
