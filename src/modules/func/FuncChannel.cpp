#include "modules/func/FuncChannel.h"

#include "modules/controls/CommonConfig.h"
#include "modules/controls/CommonInputs.h"

#include <utility>

namespace dhe {

static const auto minusTwoToPlusTwoRange = Range{-2.F, 2.F};
static const auto minusTenToPlusTenRange = Range{-10.F, 10.F};
static const auto zeroToFiveRange = Range{0.F, 5.F};

auto const FuncChannel::offsetRanges = std::array<Range const *, 4>{&zeroToFiveRange, &bipolarSignalRange,
                                                                    &unipolarSignalRange, &minusTenToPlusTenRange};

auto const FuncChannel::multiplierRanges
    = std::array<Range const *, 4>{&attenuatorRange, &attenuverterRange, &gainRange, &minusTwoToPlusTwoRange};

class FuncOperandKnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getLabel() -> std::string override { return channel->currentOperandName(); }

  auto getDisplayValue() -> float override {
    auto const rotation = getValue();
    auto const operandRange = channel->currentOperandRange();
    auto const operand = operandRange->scale(rotation);
    return operand;
  }

  void setDisplayValue(float operand) override {
    auto const operandRange = channel->currentOperandRange();
    auto const rotation = operandRange->normalize(operand);
    setValue(rotation);
  }

  void setChannel(FuncChannel const *newChannel) { channel = newChannel; }

private:
  FuncChannel const *channel;
};

FuncChannel::FuncChannel(rack::engine::Module *module, int inputId, int operandKnobId, int outputId,
                         int operationSwitchId, int offsetRangeSwitchId, int multiplierRangeSwitchId,
                         std::string channelName) :
    module{module},
    channelName{std::move(channelName)},
    inputId{inputId},
    multiplierRangeSwitchId{multiplierRangeSwitchId},
    offsetRangeSwitchId{offsetRangeSwitchId},
    operandKnobId{operandKnobId},
    operationSwitchId{operationSwitchId},
    outputId{outputId} {}

void FuncChannel::config() {
  module->configParam<FuncOperandKnobParamQuantity>(operandKnobId, 0.F, 1.F, centeredRotation);
  configToggle<2>(module, operationSwitchId, "Operator" + channelName, {"Add (offset)", "Multiply (scale)"}, 0);
  configToggle<4>(module, offsetRangeSwitchId, "Offset " + channelName + " range", {"0–5 V", "±5 V", "0–10 V", "±10 V"},
                  1);
  configToggle<4>(module, multiplierRangeSwitchId, "Multiplier " + channelName + " range", {"0–1", "±1", "0–2", "±2"},
                  2);

  auto const operandKnobParamQuantity
      = dynamic_cast<FuncOperandKnobParamQuantity *>(module->paramQuantities[operandKnobId]);

  operandKnobParamQuantity->setChannel(this);
}

auto FuncChannel::apply(float upstream) const -> float {
  auto const in = module->inputs[inputId].getNormalVoltage(upstream);
  auto const voltage = isMultiplication() ? multiply(in) : add(in);
  module->outputs[outputId].setVoltage(voltage);
  return voltage;
}

auto FuncChannel::currentOperandName() const -> std::string {
  static auto const operandNames = std::array<std::string, 2>{"Offset", "Multiplier"};
  auto const operandName = selected<std::string, 2>(module, operationSwitchId, operandNames);
  return operandName + channelName;
}

} // namespace dhe
