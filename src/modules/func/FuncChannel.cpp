#include "modules/func/FuncChannel.h"

#include <utility>

namespace dhe {

static constexpr auto attenuation_range = Range{0.f, 1.f};
static constexpr auto invertible_attenuation_range = Range{-1.f, 1.f};
static constexpr auto invertible_gain_range = Range{-2.f, 2.f};

static constexpr auto half_bipolar_range = Range{0.f, 5.f};
static constexpr auto invertible_unipolar_range = Range{-10.f, 10.f};

const std::array<Range const *, 4> FuncChannel::multiplicationRanges{
    &attenuation_range, &invertible_attenuation_range, &Gain::range,
    &invertible_gain_range};

const std::array<Range const *, 4> FuncChannel::additionRanges{
    &half_bipolar_range, &Signal::bipolar_range, &Signal::unipolar_range,
    &invertible_unipolar_range};

FuncChannel::FuncChannel(rack::engine::Module *module, int inputIndex,
                         int operandIndex, int outputIndex,
                         int operatorSwitchIndex, int additionRangeSwitchIndex,
                         int multiplicationRangeSwitchIndex)
    : input{module->inputs[inputIndex]}, operand{module->params[operandIndex]},
      output{module->outputs[outputIndex]},
      operatorSwitch{module->params[operatorSwitchIndex]},
      additionRangeSwitch{module->params[additionRangeSwitchIndex]},
      multiplicationRangeSwitch{
          module->params[multiplicationRangeSwitchIndex]} {}

auto FuncChannel::apply(float upstream) -> float {
  auto const in = input.getNormalVoltage(upstream);
  auto const rotation = operand.getValue();
  auto const isMultiplication = operatorSwitch.getValue() > 0.5f;
  auto const voltage =
      isMultiplication ? multiply(in, rotation) : add(in, rotation);
  output.setVoltage(voltage);
  return voltage;
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
} // namespace dhe
