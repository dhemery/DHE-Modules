#include "FuzzyLogicZ.h"

#include "CommonInputs.h"
#include "LevelConfig.h"
#include "Panel.h"

#include <algorithm>

namespace dhe {

FuzzyLogicZ::FuzzyLogicZ() {
  config(ParameterCount, InputCount, OutputCount);
  configParam(NotAButtons + 0, 0.F, 1.F, 0.F, "Negate A");
  configParam(NotBButtons + 0, 0.F, 1.F, 0.F, "Negate B");
  configParam(NotAButtons + 1, 0.F, 1.F, 0.F, "Negate C");
  configParam(NotBButtons + 1, 0.F, 1.F, 0.F, "Negate D");
  configLevelRangeSwitch(this, LevelRangeSwitch, "Level Range");
}

void FuzzyLogicZ::process(const rack::engine::Module::ProcessArgs & /*ignored*/) {
  auto const voltageOffset = switchPosition(params[LevelRangeSwitch]) == 1 ? 0.F : 5.F;
  for (int i = 0; i < 2; i++) {
    auto const aInput = inputs[AInputs + i].getVoltage() + voltageOffset;
    auto const bInput = inputs[BInputs + i].getVoltage() + voltageOffset;
    auto const a = buttonIsPressed(params[NotAButtons + i]) ? 10.F - aInput : aInput;
    auto const notA = 10.F - a;
    auto const b = buttonIsPressed(params[NotBButtons + i]) ? 10.F - bInput : bInput;
    auto const notB = 10.F - b;

    auto const aAndB = std::min(a, b);
    auto const aOrB = std::max(a, b);
    auto const aXorB = aOrB - aAndB;
    auto const aImpliesB = std::max(notA, b);
    auto const bImpliesA = std::max(a, notB);

    setOutputs(AndOutputs + i, NandOutputs + i, aAndB, voltageOffset);
    setOutputs(OrOutputs + i, NorOutputs + i, aOrB, voltageOffset);
    setOutputs(XorOutputs + i, XnorOutputs + i, aXorB, voltageOffset);
    setOutputs(ImplicationOutputs + i, NonimplicationOutputs + i, aImpliesB, voltageOffset);
    setOutputs(ConverseImplicationOutputs + i, ConverseNonimplicationOutputs + i, bImpliesA, voltageOffset);
  }
}

} // namespace dhe