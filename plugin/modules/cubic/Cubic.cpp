#include "Cubic.h"

#include "CommonConfig.h"

namespace dhe {
Range const Cubic::coefficientRange{-2.F, 2.F};

Cubic::Cubic() {
  config(ParameterCount, InputCount, OutputCount);
  configKnob(this, ACoefficientKnob, "x³ coefficient", "", coefficientRange);
  configKnob(this, BCoefficientKnob, "x² coefficient", "", coefficientRange);
  configKnob(this, CCoefficientKnob, "x¹ coefficient", "", coefficientRange);
  configKnob(this, DCoefficientKnob, "x⁰ coefficient", "", coefficientRange);
  configGain(this, InputGainKnob, "Input gain");
  configGain(this, OutputGainKnob, "Output gain");
}

void Cubic::process(const ProcessArgs & /*args*/) {
  auto a = coefficient(ACoefficientKnob, ACoefficientCvInput);
  auto b = coefficient(BCoefficientKnob, BCoefficientCvInput);
  auto c = coefficient(CCoefficientKnob, CCoefficientCvInput);
  auto d = coefficient(DCoefficientKnob, DCoefficientCvInput);
  auto inputGain = gain(InputGainKnob, InputGainCvInput);
  auto outputGain = gain(OutputGainKnob, OutputGainCvInput);

  auto x = inputGain * mainIn() * 0.2F;
  auto x2 = x * x;
  auto x3 = x2 * x;
  auto y = a * x3 + b * x2 + c * x + d;
  auto outputVoltage = outputGain * y * 5.F;
  sendMainOut(outputVoltage);
}
} // namespace dhe
