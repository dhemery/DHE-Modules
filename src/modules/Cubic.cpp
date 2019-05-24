#include "modules/Cubic.h"

#include "util/gain.h"
#include "util/range.h"

#include <string>

namespace dhe {

void configCoefficient(rack::engine::Module *module, int param, std::string const &name) {
  module->configParam(param, 0.f, 1.f, 0.5f, name + " coefficient", "", 0.f, 4.f, -2.f);
}

Cubic::Cubic() {
  config(ParameterCount, InputCount, OutputCount);
  configCoefficient(this, ACoefficientKnob, "x^3");
  configCoefficient(this, BCoefficientKnob, "x^2");
  configCoefficient(this, CCoefficientKnob, "x^1");
  configCoefficient(this, DCoefficientKnob, "x^0");
  configGain(InputGainKnob, "Input");
  configGain(OutputGainKnob, "Output");
}

void Cubic::process(const ProcessArgs &args) {
  auto a = coefficient(ACoefficientKnob, ACoefficientCvInput);
  auto b = coefficient(BCoefficientKnob, BCoefficientCvInput);
  auto c = coefficient(CCoefficientKnob, CCoefficientCvInput);
  auto d = coefficient(DCoefficientKnob, DCoefficientCvInput);
  auto input_gain = gain(InputGainKnob, InputGainCvInput);
  auto output_gain = gain(OutputGainKnob, OutputGainCvInput);

  auto x = input_gain * main_in() * 0.2f;
  auto x2 = x * x;
  auto x3 = x2 * x;
  auto y = a * x3 + b * x2 + c * x + d;
  auto output_voltage = output_gain * y * 5.f;
  send_main_out(output_voltage);
}

auto Cubic::coefficient(Cubic::ParameterIds knob_param, Cubic::InputIds cv_param) -> float {
  static auto constexpr coefficient_range = Range{-2.0f, 2.0f};
  return coefficient_range.scale(modulated(knob_param, cv_param));
}

auto Cubic::gain(const Cubic::ParameterIds knob_param, const Cubic::InputIds cv_input) -> float {
  return Gain::multiplier(modulated(knob_param, cv_input));
}

auto Cubic::main_in() -> float { return inputs[CubicInput].getVoltage(); }

void Cubic::send_main_out(float voltage) { outputs[CubicOutput].setVoltage(voltage); }
} // namespace dhe
