#include "modules/func/FuncChannel.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"

namespace dhe {
const std::array<Range const *, 4> multiplierRanges{&attenuator::range, &attenuverter::range, &gain::range,
                                                    &gain::invertibleRange};

const std::array<Range const *, 4> offsetRanges{&level::rectifiedBipolarRange, &level::bipolarRange,
                                                &level::unipolarRange, &level::invertibleUnipolarRange};

class FuncOperandKnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override {
    assert(range);
    auto const rotation = getValue();
    assert(rotation < 1000.F);
    auto const selectedRange = range();
    assert(selectedRange);
    auto const upperBound = selectedRange->upperBound;
    assert(upperBound < 1000.F);
    return selectedRange->scale(rotation);
    assert(false);
  }

  void setDisplayValue(float operand) override {
    assert(range);
    setValue(range()->normalize(operand));
  }
  std::function<Range const *()> range{};
};

FuncChannel::FuncChannel(rack::engine::Module *module, int inputIndex, int operandIndex, int outputIndex,
                         int operationSwitchIndex, int offsetRangeSwitchIndex, int multiplierRangeSwitchIndex,
                         std::string const &channelDescription) :
    input{&module->inputs[inputIndex]},
    operand{&module->params[operandIndex]},
    output{&module->outputs[outputIndex]},
    operationSwitch{&module->params[operationSwitchIndex]} {
  module->configParam<FuncOperandKnobParamQuantity>(operandIndex, 0.F, 1.F, knob::centered,
                                                    "Amount" + channelDescription);
  toggle::config<2>(module, operationSwitchIndex, "Operation" + channelDescription,
                    {"Add (offset)", "Multiply (scale)"}, 0);
  toggle::config<4>(module, offsetRangeSwitchIndex, "Offset range" + channelDescription,
                    {"0–5 V", "±5 V", "0–10 V", "±10 V"}, 1);
  toggle::config<4>(module, multiplierRangeSwitchIndex, "Multiplier range" + channelDescription,
                    {"0–1", "±1", "0–2", "±2"}, 2);

  offsetRange = range::selector<4>(module, offsetRangeSwitchIndex, offsetRanges);
  multiplierRange = range::selector<4>(module, multiplierRangeSwitchIndex, multiplierRanges);

  assert(offsetRange);
  assert(offsetRange());
  assert(multiplierRange);
  assert(multiplierRange());

  auto const operandKnobParamQuantity
      = dynamic_cast<FuncOperandKnobParamQuantity *>(module->paramQuantities[operandIndex]);

  assert(operandKnobParamQuantity);

  auto const operandRange = [this]() -> Range const * {
    assert(offsetRange);
    assert(multiplierRange);
    assert(operationSwitch);
    auto const isMultiplication = operationSwitch->getValue() > 0.5;
    return isMultiplication ? multiplierRange() : offsetRange();
  };

  assert(operandRange());
  operandKnobParamQuantity->range = operandRange;
  assert(operandKnobParamQuantity->range);
  assert(operandKnobParamQuantity->range());
  assert(operandKnobParamQuantity->getDisplayValue() < 1000.F);
}

auto FuncChannel::apply(float upstream) -> float {
  auto const in = input->getNormalVoltage(upstream);
  auto const rotation = operand->getValue();
  auto const isMultiplication = operationSwitch->getValue() > 0.5F;
  auto const voltage = isMultiplication ? multiply(in, rotation) : add(in, rotation);
  output->setVoltage(voltage);
  return voltage;
}

auto FuncChannel::add(float in, float rotation) const -> float { return in + offsetRange()->scale(rotation); }

auto FuncChannel::multiply(float in, float rotation) const -> float { return in * multiplierRange()->scale(rotation); }
} // namespace dhe
