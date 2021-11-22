#include "init.h"
#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace truth {
void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Module<2>, Panel<Truth2>>("Truth2"));
  plugin->addModel(rack::createModel<Module<3>, Panel<Truth3>>("Truth3"));
  plugin->addModel(rack::createModel<Module<4>, Panel<Truth4>>("Truth4"));
}
} // namespace truth
} // namespace dhe
