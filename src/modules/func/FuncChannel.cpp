#include "modules/func/FuncChannel.h"

namespace dhe {

static constexpr auto attenuationRange = Range{0.f, 1.f};
static constexpr auto invertibleAttenuationRange = Range{-1.f, 1.f};
static constexpr auto invertibleGainRange = Range{-2.f, 2.f};

static constexpr auto halfBipolarRange = Range{0.f, 5.f};
static constexpr auto invertibleUnipolarRange = Range{-10.f, 10.f};

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
  auto const isMultiplication = operatorSwitch->getValue() > 0.5f;
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
