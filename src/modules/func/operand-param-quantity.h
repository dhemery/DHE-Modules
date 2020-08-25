#pragma once

#include "operation.h"

#include <engine/ParamQuantity.hpp>

namespace dhe {
namespace func {
template <typename Controls>
class OperandParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getLabel() -> std::string override {
    static auto operand_labels =
        std::array<std::string, 2>{"Offset", "Multiplier"};
    return operand_labels[static_cast<int>(operation())] + channel_name_;
  }

  auto getDisplayValue() -> float override {
    auto const rotation = getValue();
    auto const &operand_range = current_operand_range();
    auto const operand = operand_range.scale(rotation);
    return operand;
  }

  void setDisplayValue(float operand) override {
    auto const &operand_range = current_operand_range();
    auto const rotation = operand_range.normalize(operand);
    setValue(rotation);
  }

  void configure(Controls const *controls, int channel,
                 const std::string &channel_name) {
    controls_ = controls;
    channel_ = channel;
    channel_name_ = channel_name;
  }

private:
  auto current_operand_range() -> Range {
    return operation() == Operation::Multiply ? multiplier_range()
                                              : offset_range();
  }

  auto operation() const -> Operation { return controls_->operation(channel_); }

  auto multiplier_range() const -> Range {
    return controls_->multiplier_range(channel_);
  }

  auto offset_range() const -> Range {
    return controls_->offset_range(channel_);
  }

  int channel_{0};
  std::string channel_name_;
  Controls const *controls_{};
};
} // namespace func
} // namespace dhe
