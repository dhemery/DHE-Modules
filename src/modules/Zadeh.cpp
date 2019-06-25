#include "modules/Zadeh.h"

#include "modules/controls/CommonConfig.h"
#include "modules/controls/CommonInputs.h"

#include <algorithm>
#include <panels/Panel.h>

namespace dhe {

Zadeh::Zadeh() {
  config(ParameterCount, InputCount, OutputCount);
  configParam(NotAButtons + 0, 0.F, 1.F, 0.F, "Negate A");
  configParam(NotBButtons + 0, 0.F, 1.F, 0.F, "Negate B");
  configParam(NotAButtons + 1, 0.F, 1.F, 0.F, "Negate C");
  configParam(NotBButtons + 1, 0.F, 1.F, 0.F, "Negate D");
}

void Zadeh::process(const rack::engine::Module::ProcessArgs & /*ignored*/) {
  for (int i = 0; i < 2; i++) {
    auto const aInput = inputs[AInputs + i].getVoltage();
    auto const bInput = inputs[BInputs + i].getVoltage();
    auto const a = buttonIsPressed(this, NotAButtons + i) ? 10.F - aInput : aInput;
    auto const b = buttonIsPressed(this, NotBButtons + i) ? 10.F - bInput : bInput;

    auto const aAndB = std::min(a, b);
    auto const aOrB = std::max(a, b);
    auto const aXorB = a + b - (aAndB + aAndB);
    auto const aImpliesB = 10.F - std::min(a, 10.F - b);
    auto const bImpliesA = 10.F - std::min(10.F - a, b);

    setOutputs(AndOutputs + i, NandOutputs + i, aAndB);
    setOutputs(OrOutputs + i, NorOutputs + i, aOrB);
    setOutputs(XorOutputs + i, XnorOutputs + i, aXorB);
    setOutputs(AImpliesBOutputs + i, ANotImpliesBOutputs + i, aImpliesB);
    setOutputs(BImpliesAOutputs + i, BNotImpliesAOutputs + i, bImpliesA);
  }
}

} // namespace dhe
