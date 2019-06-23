#include "modules/Flogic.h"

#include "modules/controls/CommonConfig.h"
#include "modules/controls/CommonInputs.h"

#include <algorithm>
#include <panels/Panel.h>

namespace dhe {

Flogic::Flogic() {
  config(ParameterCount, InputCount, OutputCount);
  for (auto i = 0; i < 2; i++) {
    configParam(NegateAButtons + i, 0.F, 1.F, 0.F, "Negate A");
    configParam(NegateBButtons + i, 0.F, 1.F, 0.F, "Negate B");
  }
}

void Flogic::process(const rack::engine::Module::ProcessArgs & /*ignored*/) {
  for (int i = 0; i < 2; i++) {
    auto const aInput = 0.1F * inputs[AInputs + i].getVoltage();
    auto const bInput = 0.1F * inputs[BInputs + i].getVoltage();
    auto const a = buttonIsPressed(this, NegateAButtons + i) ? 1.F - aInput : aInput;
    auto const b = buttonIsPressed(this, NegateBButtons + i) ? 1.F - bInput : bInput;
    auto const max = std::max(a, b);
    auto const min = std::min(a, b);
    auto const fuzzyAnd = a * b;
    auto const fuzzyOr = a + b - fuzzyAnd;
    setOutputs(MaxOutputs + i, NegatedMaxOutputs + i, max);
    setOutputs(MinOutputs + i, NegatedMinOutputs + i, min);
    setOutputs(AndOutputs + i, NegatedAndOutputs + i, fuzzyAnd);
    setOutputs(OrOutputs + i, NegatedOrOutputs + i, fuzzyOr);
  }
}

} // namespace dhe
