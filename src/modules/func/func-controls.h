#pragma once

#include "operation.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>

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

template <int N> class FuncControls {
public:
  FuncControls(std::vector<rack::engine::Input> const &inputs,
               std::vector<rack::engine::Param> const &params,
               std::vector<rack::engine::Output> &outputs)
      : inputs_{inputs}, params_{params}, outputs_{outputs} {}

  auto input(int channel, float voltage_if_disconnected) const -> float {
    return const_cast<rack::engine::Input &>(inputs_[FuncInput + channel])
        .getNormalVoltage(voltage_if_disconnected);
  }

  auto multiplier_range(int channel) const -> Range {
    return selected_range<4>(params_[MultiplierRangeSwitch + channel],
                             multiplier_ranges);
  }

  auto offset_range(int channel) const -> Range {
    return selected_range<4>(params_[OffsetRangeSwitch + channel],
                             offset_ranges);
  }

  auto operand(int channel) const -> float {
    return rotation_of(params_[AmountKnob + channel]);
  }

  auto operation(int channel) const -> Operation {
    return static_cast<Operation>(
        position_of(params_[OperationSwitch + channel]));
  }

  void output(int channel, float voltage) {
    outputs_[FuncOutput + channel].setVoltage(voltage);
  }

  enum ParameterIds {
    ENUMS(AmountKnob, N),
    ENUMS(OperationSwitch, N),
    ENUMS(OffsetRangeSwitch, N),
    ENUMS(MultiplierRangeSwitch, N),
    ParameterCount
  };

  enum InputIds { FuncInput, InputCount = FuncInput + N };

  enum OutputIds { FuncOutput, OutputCount = FuncOutput + N };

private:
  std::vector<rack::engine::Input> const &inputs_;
  std::vector<rack::engine::Param> const &params_;
  std::vector<rack::engine::Output> &outputs_;
};
} // namespace func
} // namespace dhe
