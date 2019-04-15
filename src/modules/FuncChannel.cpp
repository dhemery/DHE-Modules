#include <array>

#include "modules/FuncChannel.h"

namespace DHE {

static const auto multiplication_ranges = std::array<Range const *, 4>{
    &attenuation_range, &invertible_attenuation_range, &Gain::range,
    &invertible_gain_range};

static const auto addition_ranges = std::array<Range const *, 4>{
    &half_bipolar_range, &Signal::bipolar_range, &Signal::unipolar_range,
    &invertible_unipolar_range};

FuncChannel::FuncChannel(rack::Module *module, int input, int amount_knob_param,
                         int output)
    : input_port{module->inputs[input]},
      amount{module->params[amount_knob_param].value},
      output{module->outputs[output].value} {}

auto FuncChannel::adjust(float upstream) -> float {
  auto input = input_port.active ? input_port.value : upstream;
  if (is_multiplication) {
    output = input * multiplication_range->scale(amount);
  } else {
    output = input + addition_range->scale(amount);
  }
  return output;
}

void FuncChannel::set_operator(bool is_multiplication) {
  this->is_multiplication = is_multiplication;
}

void FuncChannel::set_addition_range(int selection) {
  addition_range = addition_ranges[selection];
}

void FuncChannel::set_multiplication_range(int selection) {
  multiplication_range = multiplication_ranges[selection];
}
} // namespace DHE
