#include "modules/FuzzyLogicZ.h"

#include "modules/controls/CommonInputs.h"

#include <algorithm>
#include <panels/Panel.h>

namespace dhe {

FuzzyLogicZ::FuzzyLogicZ() {
  config(ParameterCount, InputCount, OutputCount);
  configParam(NotAButtons + 0, 0.F, 1.F, 0.F, "Negate A");
  configParam(NotBButtons + 0, 0.F, 1.F, 0.F, "Negate B");
  configParam(NotAButtons + 1, 0.F, 1.F, 0.F, "Negate C");
  configParam(NotBButtons + 1, 0.F, 1.F, 0.F, "Negate D");
}

void FuzzyLogicZ::process(const rack::engine::Module::ProcessArgs & /*ignored*/) {
  for (int i = 0; i < 2; i++) {
    auto const aInput = inputs[AInputs + i].getVoltage();
    auto const bInput = inputs[BInputs + i].getVoltage();
    auto const a = buttonIsPressed(this, NotAButtons + i) ? 10.F - aInput : aInput;
    auto const notA = 10.F - a;
    auto const b = buttonIsPressed(this, NotBButtons + i) ? 10.F - bInput : bInput;
    auto const notB = 10.F - b;

    auto const conjunction = std::min(a, b);
    auto const disjunction = std::max(a, b);
    auto const exclusiveDisjunction = disjunction - conjunction;
    auto const implication = 10.F - std::min(a, notB);
    auto const converseImplication = 10.F - std::min(notA, b);

    setOutputs(AndOutputs + i, NandOutputs + i, conjunction);
    setOutputs(OrOutputs + i, NorOutputs + i, disjunction);
    setOutputs(XorOutputs + i, XnorOutputs + i, exclusiveDisjunction);
    setOutputs(ImplicationOutputs + i, NonimplicationOutputs + i, implication);
    setOutputs(ConverseImplicationOutputs + i, ConverseNonimplicationOutputs + i, converseImplication);
  }
}

} // namespace dhe
