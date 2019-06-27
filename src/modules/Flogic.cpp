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

  auto const sum = a + b;
  auto const zAnd = std::min(a, b);
  auto const zOr = std::max(a, b);
  auto const zXor = zOr - zAnd;
  auto const zImplication = 10.F - std::min(a, notB);
  auto const zConverseImplication = 10.F - std::min(notA, b);
  auto const hAnd = a * b * 0.1F;
  auto const hOr = sum - hAnd;
  auto const hXor = hOr - hAnd;
  auto const hImplication = notA + hAnd;
  auto const hConverseImplication = notB + hAnd;

  setOutputs(ZAndOutput, ZNandOutput, zAnd);
  setOutputs(ZOrOutput, ZNorOutput, zOr);
  setOutputs(ZXorOutput, ZXnorOutput, zXor);
  setOutputs(ZImplicationOutput, ZNonimplicationOutput, zImplication);
  setOutputs(ZConverseImplicationOutput, ZConverseNonimplicationOutput, zConverseImplication);

  setOutputs(HAndOutput, HNandOutput, hAnd);
  setOutputs(HOrOutput, HNorOutput, hOr);
  setOutputs(HXorOutput, HXnorOutput, hXor);
  setOutputs(HImplicationOutput, HNonimplicationOutput, hImplication);
  setOutputs(HConverseImplicationOutput, HConverseNonimplicationOutput, hConverseImplication);
}

} // namespace dhe
