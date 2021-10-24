#pragma once

#include "control-ids.h"
#include "operation.h"

#include "components/range.h"

#include <array>
#include <vector>

namespace dhe {
namespace func {
static auto constexpr minus_two_to_plus_two_range = Range{-2.F, 2.F};
static auto constexpr minus_ten_to_plus_ten_range = Range{-10.F, 10.F};
static auto constexpr zero_to_five_range = Range{0.F, 5.F};

static auto constexpr offset_ranges = std::array<Range const, 4>{
    zero_to_five_range, bipolar_signal_range, unipolar_signal_range,
    minus_ten_to_plus_ten_range};

static auto constexpr multiplier_ranges =
    std::array<Range const, 4>{attenuator_range, attenuverter_range, gain_range,
                               minus_two_to_plus_two_range};

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
    return selected_range<4>(params_[Param::MultiplierRange + channel],
                             multiplier_ranges);
  }

  auto offset_range(int channel) const -> Range {
    return selected_range<4>(params_[Param::OffsetRange + channel],
                             offset_ranges);
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
