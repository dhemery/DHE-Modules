#include "modules/Flogic.h"

#include "modules/controls/CommonConfig.h"
#include "modules/controls/CommonInputs.h"

#include <algorithm>
#include <panels/Panel.h>

namespace dhe {

Flogic::Flogic() {
  config(ParameterCount, InputCount, OutputCount);
  for (auto i = 0; i < 2; i++) {
    configParam(ANotButtons + i, 0.F, 1.F, 0.F, "Negate A");
    configParam(BNotButtons + i, 0.F, 1.F, 0.F, "Negate B");
  }
}

void Flogic::process(const rack::engine::Module::ProcessArgs & /*ignored*/) {
  for (int i = 0; i < 2; i++) {
    auto const aInput = inputs[AInputs + i].getVoltage();
    auto const bInput = inputs[BInputs + i].getVoltage();
    auto const a = buttonIsPressed(this, ANotButtons + i) ? 10.F - aInput : aInput;
    auto const b = buttonIsPressed(this, BNotButtons + i) ? 10.F - bInput : bInput;
    auto const zOr = std::max(a, b);
    auto const zAnd = std::min(a, b);
    auto const pAnd = a * b * 0.1F;
    auto const pOr = a + b - pAnd;
    setOutputs(PAndOutputs + i, PNandOutputs + i, pAnd);
    setOutputs(POrOutputs + i, PNorOutputs + i, pOr);
    setOutputs(ZOrOutputs + i, ZNorOutputs + i, zOr);
    setOutputs(ZAndOutputs + i, ZNandOutputs + i, zAnd);
  }
}

} // namespace dhe
