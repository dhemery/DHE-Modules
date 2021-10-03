#pragma once

#include "operation.h"

#include <engine/ParamQuantity.hpp>

namespace dhe {
namespace func {
template <template <int> class Controls, int N>
class OperandParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getLabel() -> std::string override {
    static auto operand_labels =
        N == 1 ? std::array<std::string, 2>{"Offset", "Multiplier"}
               : std::array<std::string, 2>{" offset", " multiplier"};
    return channel_name_ + operand_labels[static_cast<int>(operation())];
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

  void configure(Controls<N> const *controls, int channel,
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
  Controls<N> const *controls_{};
};
} // namespace func
} // namespace dhe
