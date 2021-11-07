#pragma once

#include "control-ids.h"
#include "operation.h"

#include "components/range.h"

#include <array>
#include <vector>

namespace dhe {
namespace func {

static inline auto offset_ranges() -> std::vector<Range> const & {
  static auto constexpr minus_ten_to_plus_ten_range = Range{-10.F, 10.F};
  static auto constexpr zero_to_five_range = Range{0.F, 5.F};
  static auto const ranges =
      std::vector<Range>{zero_to_five_range, bipolar_signal_range,
                         unipolar_signal_range, minus_ten_to_plus_ten_range};
  return ranges;
}

static inline auto multiplier_ranges() -> std::vector<Range> const & {
  static auto constexpr minus_two_to_plus_two_range = Range{-2.F, 2.F};
  static auto const ranges =
      std::vector<Range>{Attenuator::range, Attenuverter::range, Gain::range,
                         minus_two_to_plus_two_range};
  return ranges;
}
template <typename TParam, typename TInput, typename TOutput, int N>
struct Signals {
  static auto constexpr channel_count = N;
  using Input = InputIds<N>;
  using Param = ParamIds<N>;
  using Output = OutputIds<N>;

  Signals(std::vector<TParam> &params, std::vector<TInput> &inputs,
          std::vector<TOutput> &outputs)
      : params_{params}, inputs_{inputs}, outputs_{outputs} {}

  auto input(int channel, float voltage_if_disconnected) const -> float {
    return inputs_[Input::Channel + channel].getNormalVoltage(
        voltage_if_disconnected);
  }

  auto multiplier_range(int channel) const -> Range {
    auto const selection =
        position_of(params_[Param::MultiplierRange + channel]);
    return multiplier_ranges()[selection];
  }

  auto offset_range(int channel) const -> Range {
    auto const selection = position_of(params_[Param::OffsetRange + channel]);
    return offset_ranges()[selection];
  }

  auto operand(int channel) const -> float {
    return rotation_of(params_[Param::Operand + channel]);
  }

  auto operation(int channel) const -> Operation {
    return static_cast<Operation>(
        position_of(params_[Param::Operation + channel]));
  }

  void output(int channel, float voltage) {
    outputs_[Output::Channel + channel].setVoltage(voltage);
  }

private:
  std::vector<TParam> &params_;
  std::vector<TInput> &inputs_;
  std::vector<TOutput> &outputs_;
};
} // namespace func
} // namespace dhe
