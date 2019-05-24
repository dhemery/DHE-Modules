#include "modules/func/FuncChannel.h"

namespace dhe {

static constexpr auto attenuationRange = Range{0.F, 1.F};
static constexpr auto invertibleAttenuationRange = Range{-1.F, 1.F};
static constexpr auto invertibleGainRange = Range{-2.F, 2.F};

static constexpr auto halfBipolarRange = Range{0.F, 5.F};
static constexpr auto invertibleUnipolarRange = Range{-10.F, 10.F};

const std::array<Range const *, 4> FuncChannel::multiplicationRanges{&attenuationRange, &invertibleAttenuationRange,
                                                                     &Gain::range, &invertibleGainRange};

const std::array<Range const *, 4> FuncChannel::additionRanges{&halfBipolarRange, &signal::bipolarRange,
                                                               &signal::unipolarRange, &invertibleUnipolarRange};

FuncChannel::FuncChannel(rack::engine::Module *module, int inputIndex, int operandIndex, int outputIndex,
                         int operatorSwitchIndex, int additionRangeSwitchIndex, int multiplicationRangeSwitchIndex) :
    input{&module->inputs[inputIndex]},
    operand{&module->params[operandIndex]},
    output{&module->outputs[outputIndex]},
    operatorSwitch{&module->params[operatorSwitchIndex]},
    additionRangeSwitch{&module->params[additionRangeSwitchIndex]},
    multiplicationRangeSwitch{&module->params[multiplicationRangeSwitchIndex]} {}

auto FuncChannel::apply(float upstream) -> float {
  auto const in = input->getNormalVoltage(upstream);
  auto const rotation = operand->getValue();
  auto const isMultiplication = operatorSwitch->getValue() > 0.5F;
  auto const voltage = isMultiplication ? multiply(in, rotation) : add(in, rotation);
  output->setVoltage(voltage);
  return voltage;
}

auto FuncChannel::add(float in, float rotation) const -> float {
  auto rangeSelection = static_cast<int>(additionRangeSwitch->getValue());
  auto range = additionRanges[rangeSelection];
  return in + range->scale(rotation);
}

auto FuncChannel::multiply(float in, float rotation) const -> float {
  auto rangeSelection = static_cast<int>(multiplicationRangeSwitch->getValue());
  auto range = multiplicationRanges[rangeSelection];
  return in * range->scale(rotation);
}
} // namespace dhe
