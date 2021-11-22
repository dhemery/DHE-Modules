#pragma once

#include "components/range.h"
#include "control-ids.h"
#include "params.h"
#include "signals/basic.h"

#include <array>
#include <vector>

namespace dhe {
namespace func {

template <typename TParam, typename TInput, typename TOutput, int N>
struct Signals {
  static auto constexpr channel_count = N;
  using InputId = InputIds<N>;
  using ParamId = ParamIds<N>;
  using OutputId = OutputIds<N>;

  Signals(std::vector<TParam> &params, std::vector<TInput> &inputs,
          std::vector<TOutput> &outputs)
      : params_{params}, inputs_{inputs}, outputs_{outputs} {}

  auto input(int channel, float voltage_if_disconnected) const -> float {
    return inputs_[InputId::Channel + channel].getNormalVoltage(
        voltage_if_disconnected);
  }

  auto multiplier_range(int channel) const -> MultiplierRangeId {
    return value_of<MultiplierRangeId>(
        params_[ParamId::MultiplierRange + channel]);
  }

  auto offset_range(int channel) const -> OffsetRangeId {
    return value_of<OffsetRangeId>(params_[ParamId::OffsetRange + channel]);
  }

  auto operand(int channel) const -> float {
    return value_of(params_[ParamId::Operand + channel]);
  }

  auto operation(int channel) const -> Operation {
    return value_of<Operation>(params_[ParamId::Operation + channel]);
  }

  void output(int channel, float voltage) {
    outputs_[OutputId::Channel + channel].setVoltage(voltage);
  }

private:
  std::vector<TParam> &params_;
  std::vector<TInput> &inputs_;
  std::vector<TOutput> &outputs_;
};
} // namespace func
} // namespace dhe
