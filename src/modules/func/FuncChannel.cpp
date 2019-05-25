#include "modules/func/FuncChannel.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"

namespace dhe {

const std::array<Range const *, 4> FuncChannel::multiplierRanges{&attenuator::range, &attenuverter::range, &gain::range,
                                                                 &gain::invertibleRange};

const std::array<Range const *, 4> FuncChannel::offsetRanges{&level::rectifiedBipolarRange, &level::bipolarRange,
                                                             &level::unipolarRange, &level::invertibleUnipolarRange};

FuncChannel::FuncChannel(rack::engine::Module *module, int inputIndex, int amountIndex, int outputIndex,
                         int operationSwitchIndex, int offsetRangeSwitchIndex, int multiplierRangeSwitchIndex) :
    input{&module->inputs[inputIndex]},
    amount{&module->params[amountIndex]},
    output{&module->outputs[outputIndex]},
    operationSwitch{&module->params[operationSwitchIndex]},
    offsetRangeSwitch{&module->params[offsetRangeSwitchIndex]},
    multiplierRangeSwitch{&module->params[multiplierRangeSwitchIndex]} {}

auto FuncChannel::apply(float upstream) -> float {
  auto const in = input->getNormalVoltage(upstream);
  auto const rotation = amount->getValue();
  auto const isMultiplication = operationSwitch->getValue() > 0.5F;
  auto const voltage = isMultiplication ? multiply(in, rotation) : add(in, rotation);
  output->setVoltage(voltage);
  return voltage;
}

auto FuncChannel::add(float in, float rotation) const -> float {
  auto rangeSelection = static_cast<int>(offsetRangeSwitch->getValue());
  auto range = offsetRanges[rangeSelection];
  return in + range->scale(rotation);
}

auto FuncChannel::multiply(float in, float rotation) const -> float {
  auto rangeSelection = static_cast<int>(multiplierRangeSwitch->getValue());
  auto range = multiplierRanges[rangeSelection];
  return in * range->scale(rotation);
}
} // namespace dhe
