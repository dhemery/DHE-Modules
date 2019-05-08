#include <utility>

#include <array>

#include "modules/func/FuncChannel.h"

namespace DHE {

static const auto multiplicationRanges = std::array<Range const *, 4>{
    &attenuation_range, &invertible_attenuation_range, &Gain::range,
    &invertible_gain_range};

static const auto additionRanges = std::array<Range const *, 4>{
    &half_bipolar_range, &Signal::bipolar_range, &Signal::unipolar_range,
    &invertible_unipolar_range};

FuncChannel::FuncChannel(rack::engine::Module *module, int inputIndex,
                         int operandIndex, int outputIndex,
                         int operatorSwitchIndex, int additionRangeSwitchIndex,
                         int multiplicationRangeSwitchIndex,
                         std::function<void(FuncOperator)> onOperatorChange)
    : input{module->inputs[inputIndex]}, operand{module->params[operandIndex]},
      output{module->outputs[outputIndex]},
      operatorSwitch{module->params[operatorSwitchIndex]},
      additionRangeSwitch{module->params[additionRangeSwitchIndex]},
      multiplicationRangeSwitch{module->params[multiplicationRangeSwitchIndex]},
      onOperatorChange{std::move(onOperatorChange)} {}

auto FuncChannel::apply(float upstream) -> float {
  auto const in = input.getVoltage();
  auto const rotation = operand.getValue();
  setOperator();
  return op == ADD ? add(in, rotation) : multiply(in, rotation);
}

auto FuncChannel::add(float in, float rotation) const -> float {
  auto rangeSelection = static_cast<int>(additionRangeSwitch.getValue());
  auto range = additionRanges[rangeSelection];
  return in + range->scale(rotation);
}

auto FuncChannel::multiply(float in, float rotation) const -> float {
  auto rangeSelection = static_cast<int>(multiplicationRangeSwitch.getValue());
  auto range = multiplicationRanges[rangeSelection];
  return in * range->scale(rotation);
}

void FuncChannel::setOperator() {
  auto selectedOp = operatorSwitch.getValue() > 0.5f ? MULTIPLY : ADD;
  if (selectedOp == op) {
    return;
  }
  op = selectedOp;
  onOperatorChange(op);
}
} // namespace DHE
