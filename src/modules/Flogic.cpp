#include "modules/Flogic.h"

#include "modules/controls/CommonInputs.h"

#include <algorithm>
#include <panels/Panel.h>

namespace dhe {

Flogic::Flogic() {
  config(ParameterCount, InputCount, OutputCount);
  configParam(NotAButton, 0.F, 1.F, 0.F, "Negate A");
  configParam(NotBButton, 0.F, 1.F, 0.F, "Negate B");
}

void Flogic::process(const rack::engine::Module::ProcessArgs & /*ignored*/) {
  auto const aInput = inputs[AInput].getVoltage();
  auto const bInput = inputs[BInput].getVoltage();
  auto const a = buttonIsPressed(this, NotAButton) ? 10.F - aInput : aInput;
  auto const notA = 10.F - a;
  auto const b = buttonIsPressed(this, NotBButton) ? 10.F - bInput : bInput;
  auto const notB = 10.F - b;

  auto const aAndB = std::min(a, b);
  auto const aOrB = std::max(a, b);
  auto const aXorB = a + b - (aAndB + aAndB);
  auto const probAAndB = a * b * 0.1F;
  auto const probAOrb = a + b - probAAndB;
  auto const aImpliesB = 10.F - std::min(a, notB);
  auto const bImpliesA = 10.F - std::min(notA, b);

  setOutputs(AndOutput, NandOutput, aAndB);
  setOutputs(OrOutput, NorOutput, aOrB);
  setOutputs(XorOutput, XnorOutput, aXorB);
  setOutputs(PAndOutput, PNandOutput, probAAndB);
  setOutputs(POrOutput, PNorOutput, probAOrb);
  setOutputs(AImpliesBOutput, ANotImpliesBOutput, aImpliesB);
  setOutputs(BImpliesAOutput, BNotImpliesAOutput, bImpliesA);

  outputs[NotAOutput].setVoltage(notA);
  outputs[NotBOutput].setVoltage(notB);
}

} // namespace dhe
