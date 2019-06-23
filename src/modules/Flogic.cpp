#include "modules/Flogic.h"

#include "modules/controls/CommonConfig.h"
#include "modules/controls/CommonInputs.h"

#include <panels/Panel.h>
#include <string>

namespace dhe {

Flogic::Flogic() {
  config(ParameterCount, InputCount, OutputCount);
}

void Flogic::process(const rack::engine::Module::ProcessArgs & /*ignored*/) {
}

} // namespace dhe
