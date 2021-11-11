#pragma once

#include "control-ids.h"
#include "params.h"

#include "components/range.h"

#include <array>
#include <vector>

namespace dhe {
namespace func {

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

  auto multiplier_range(int channel) const -> Multipliers::Selection {
    return selection_of<Multipliers::Selection>(
        params_[Param::MultiplierRange + channel]);
  }

  auto offset_range(int channel) const -> Offsets::Selection {
    return selection_of<Offsets::Selection>(
        params_[Param::OffsetRange + channel]);
  }

  auto operand(int channel) const -> float {
    return value_of(params_[Param::Operand + channel]);
  }

  auto operation(int channel) const -> Operations::Selection {
    return selection_of<Operations::Selection>(
        params_[Param::Operation + channel]);
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
