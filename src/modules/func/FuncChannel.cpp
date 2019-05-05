#include <array>

#include "modules/func/FuncChannel.h"

namespace DHE {

static const auto multiplication_ranges = std::array<Range const *, 4>{
    &attenuation_range, &invertible_attenuation_range, &Gain::range,
    &invertible_gain_range};

static const auto addition_ranges = std::array<Range const *, 4>{
    &half_bipolar_range, &Signal::bipolar_range, &Signal::unipolar_range,
    &invertible_unipolar_range};

FuncChannel::FuncChannel(rack::engine::Module *module, int inputIndex, int operandIndex,
                         int outputIndex)
    : input{module->inputs[inputIndex]},
      operand{module->params[operandIndex]},
      output{module->outputs[outputIndex]} {}

auto FuncChannel::adjust(float upstream) -> float {
  auto inputVoltage = input.getNormalVoltage(upstream);
  auto operandValue = operand.getValue();
  if (is_multiplication) {
    return inputVoltage * multiplication_range->scale(operandValue);
  } else {
    return inputVoltage + addition_range->scale(operandValue);
  }
}
} // namespace DHE
