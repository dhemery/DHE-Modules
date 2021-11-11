#pragma once
#include "./operation.h"

#include <string>
#include <vector>

namespace dhe {
namespace func {

struct OffsetRanges {
  enum Selection { Unipolar5, Bipolar, Unipolar, Bipolar10 };
  static constexpr auto stepper_slug = "offset-range";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"0–5 V", "±5 V", "0–10 V", "±10 V"};
    return labels;
  }
};

struct MultiplierRanges {
  enum Selection { Attenuator, Attenuverter, Gain, Gainuverter };
  static constexpr auto stepper_slug = "multiplier-range";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"0–1", "±1", "0–2", "±2"};
    return labels;
  }
};

struct Operators {
  using Selection = Operation;
  static constexpr auto stepper_slug = "toggle-2";

  static inline auto labels() -> std::vector<std::string> const & {
    static const auto labels =
        std::vector<std::string>{"Add (offset)", "Multiply (scale)"};
    return labels;
  }
};

template <typename Signals>
struct OperandParamQuantity : public rack::engine::ParamQuantity {
  auto getLabel() -> std::string override {
    static auto operand_labels =
        Signals::channel_count == 1
            ? std::array<std::string, 2>{"Offset", "Multiplier"}
            : std::array<std::string, 2>{" offset", " multiplier"};
    const auto op = static_cast<int>(operation());
    return channel_name_ + operand_labels[op];
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

  void configure(Signals const *signals, int channel,
                 const std::string &channel_name) {
    signals_ = signals;
    channel_ = channel;
    channel_name_ = channel_name;
  }

private:
  auto current_operand_range() -> Range {
    return operation() == Operation::Multiply ? multiplier_range()
                                              : offset_range();
  }

  auto operation() const -> Operation { return signals_->operation(channel_); }

  auto multiplier_range() const -> Range {
    return signals_->multiplier_range(channel_);
  }

  auto offset_range() const -> Range {
    return signals_->offset_range(channel_);
  }

  int channel_{0};
  std::string channel_name_;
  Signals const *signals_{};
};
} // namespace func

} // namespace dhe
