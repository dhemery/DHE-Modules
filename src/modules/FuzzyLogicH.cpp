#include "modules/FuzzyLogicH.h"

#include "modules/controls/CommonInputs.h"

#include <algorithm>
#include <panels/Panel.h>

namespace dhe {

FuzzyLogicH::FuzzyLogicH() {
  config(ParameterCount, InputCount, OutputCount);
  configParam(NotAButtons + 0, 0.F, 1.F, 0.F, "Negate A");
  configParam(NotBButtons + 0, 0.F, 1.F, 0.F, "Negate B");
  configParam(NotAButtons + 1, 0.F, 1.F, 0.F, "Negate C");
  configParam(NotBButtons + 1, 0.F, 1.F, 0.F, "Negate D");
}

void FuzzyLogicH::process(const rack::engine::Module::ProcessArgs & /*ignored*/) {
  for (int i = 0; i < 2; i++) {
    auto const aInput = inputs[AInputs + i].getVoltage();
    auto const bInput = inputs[BInputs + i].getVoltage();
    auto const a = buttonIsPressed(this, NotAButtons + i) ? 10.F - aInput : aInput;
    auto const notA = 10.F - a;
    auto const b = buttonIsPressed(this, NotBButtons + i) ? 10.F - bInput : bInput;
    auto const notB = 10.F - b;

    auto const aAndB = a * b * 0.1F;
    auto const aOrB = a + b - aAndB;
    auto const aXorB = aOrB - aAndB;
    auto const aImpliesB = notA + aAndB;
    auto const bImpliesA = notB + aAndB;

    setOutputs(AndOutputs + i, NandOutputs + i, aAndB);
    setOutputs(OrOutputs + i, NorOutputs + i, aOrB);
    setOutputs(XorOutputs + i, XnorOutputs + i, aXorB);
    setOutputs(ImplicationOutputs + i, NonimplicationOutputs + i, aImpliesB);
    setOutputs(ConverseImplicationOutputs + i, ConverseNonimplicationOutputs + i, bImpliesA);
  }
}

} // namespace dhe
