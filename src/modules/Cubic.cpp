#include "modules/Cubic.h"

#include "modules/controls/Controls.h"
#include "util/Gain.h"

#include <string>

namespace dhe {

void configCoefficient(rack::engine::Module *module, int param, std::string const &name) {
  module->configParam(param, 0.F, 1.F, 0.5F, name, "", 0.F, 4.F, -2.F);
}

Cubic::Cubic() {
  config(ParameterCount, InputCount, OutputCount);
  configCoefficient(this, ACoefficientKnob, "x³ coefficient");
  configCoefficient(this, BCoefficientKnob, "x² coefficient");
  configCoefficient(this, CCoefficientKnob, "x¹ coefficient");
  configCoefficient(this, DCoefficientKnob, "x⁰ coefficient");
  gain_knob::config(this, InputGainKnob, "Input gain");
  gain_knob::config(this, OutputGainKnob, "Output gain");
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

auto Cubic::coefficient(Cubic::ParameterIds knobParam, Cubic::InputIds cvParam) -> float {
  static auto constexpr coefficientRange = Range{-2.0F, 2.0F};
  return coefficientRange.scale(modulated(knobParam, cvParam));
}

auto Cubic::gain(const Cubic::ParameterIds knobParam, const Cubic::InputIds cvInput) -> float {
  return gain::multiplier(modulated(knobParam, cvInput));
}

auto Cubic::mainIn() -> float { return inputs[CubicInput].getVoltage(); }

void Cubic::sendMainOut(float voltage) { outputs[CubicOutput].setVoltage(voltage); }
} // namespace dhe
