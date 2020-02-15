#pragma once

#include "Operation.h"

#include <engine/ParamQuantity.hpp>

namespace dhe {
namespace func {
  template <typename Controls> class OperandParamQuantity : public rack::engine::ParamQuantity {
  public:
    auto getLabel() -> std::string override {
      static auto operandLabels = std::array<std::string, 2>{"Offset", "Multiplier"};
      return operandLabels[static_cast<int>(operation())] + channelName;
    }

    auto getDisplayValue() -> float override {
      auto const rotation = getValue();
      auto const operandRange = currentOperandRange();
      auto const operand = operandRange->scale(rotation);
      return operand;
    }

    void setDisplayValue(float operand) override {
      auto const operandRange = currentOperandRange();
      auto const rotation = operandRange->normalize(operand);
      setValue(rotation);
    }

    void configure(Controls const *theControls, int theChannel, const std::string &theChannelName) {
      controls = theControls;
      channel = theChannel;
      channelName = theChannelName;
    }

  private:
    auto currentOperandRange() -> Range const * {
      return operation() == Operation::Multiply ? multiplierRange() : offsetRange();
    }

    auto operation() const -> Operation { return controls->operation(channel); }

    auto multiplierRange() const -> Range const * { return controls->multiplierRange(channel); }

    auto offsetRange() const -> Range const * { return controls->offsetRange(channel); }

    int channel{0};
    std::string channelName;
    Controls const *controls{};
  };
} // namespace func
} // namespace dhe
