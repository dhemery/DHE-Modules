#include "init.h"
#include "func1-panel.h"
#include "func6-panel.h"
#include "module.h"

namespace dhe {
namespace func {
static auto default_value = BipolarVoltage::scale(0.F);

void init(rack::plugin::Plugin *plugin) {
  plugin->addModel(rack::createModel<Module<1>, Func1Panel>("Func"));
  plugin->addModel(rack::createModel<Module<6>, Func6Panel>("Func6")); // NOLINT
}
} // namespace func
} // namespace dhe
