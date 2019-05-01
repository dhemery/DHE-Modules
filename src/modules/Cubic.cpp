#include "modules/Cubic.h"
#include "util/gain.h"
#include "util/range.h"

namespace DHE {

Cubic::Cubic() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

void Cubic::step() {
  auto a = coefficient(A_KNOB, A_CV);
  auto b = coefficient(B_KNOB, B_CV);
  auto c = coefficient(C_KNOB, C_CV);
  auto d = coefficient(D_KNOB, D_CV);
  auto input_gain = gain(INPUT_GAIN_KNOB, INPUT_GAIN_CV);
  auto output_gain = gain(OUTPUT_GAIN_KNOB, OUTPUT_GAIN_CV);

  auto x = input_gain * main_in() * 0.2f;
  auto x2 = x * x;
  auto x3 = x2 * x;
  auto y = a * x3 + b * x2 + c * x + d;
  auto output_voltage = output_gain * y * 5.f;
  send_main_out(output_voltage);
}

auto Cubic::coefficient(Cubic::ParameterIds knob_param,
                        Cubic::InputIds cv_param) const -> float {
  static auto constexpr coefficient_range = Range{-2.0f, 2.0f};
  return coefficient_range.scale(modulated(knob_param, cv_param));
}

auto Cubic::gain(const Cubic::ParameterIds knob_param,
                 const Cubic::InputIds cv_input) const -> float {
  return Gain::multiplier(modulated(knob_param, cv_input));
}

auto Cubic::main_in() const -> float { return inputs[MAIN_IN].value; }

void Cubic::send_main_out(float voltage) { outputs[MAIN_OUT].value = voltage; }
} // namespace DHE
