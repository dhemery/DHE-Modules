#pragma once

#include "Operation.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>

namespace dhe {
namespace func {
static auto constexpr minusTwoToPlusTwoRange = Range{-2.F, 2.F};
static auto constexpr minusTenToPlusTenRange = Range{-10.F, 10.F};
static auto constexpr zeroToFiveRange = Range{0.F, 5.F};

static auto constexpr offsetRanges = std::array<Range const *, 4>{
    &zeroToFiveRange, &bipolar_signal_range, &unipolar_signal_range,
    &minusTenToPlusTenRange};

static auto constexpr multiplierRanges =
    std::array<Range const *, 4>{&attenuator_range, &attenuverter_range,
                                 &gain_range, &minusTwoToPlusTwoRange};

template <int N> class FuncControls {
public:
  FuncControls(std::vector<rack::engine::Input> const &inputs,
               std::vector<rack::engine::Param> const &params,
               std::vector<rack::engine::Output> &outputs)
      : inputs{inputs}, params{params}, outputs{outputs} {}

  auto input(int channel, float voltageIfDisconnected) const -> float {
    return const_cast<rack::engine::Input &>(inputs[FuncInput + channel])
        .getNormalVoltage(voltageIfDisconnected);
  }

  auto multiplierRange(int channel) const -> Range const * {
    return selected_range<4>(params[MultiplierRangeSwitch + channel],
                             multiplierRanges);
  }

  auto offsetRange(int channel) const -> Range const * {
    return selected_range<4>(params[OffsetRangeSwitch + channel], offsetRanges);
  }

  auto operand(int channel) const -> float {
    return rotation_of(params[AmountKnob + channel]);
  }

  auto operation(int channel) const -> Operation {
    return static_cast<Operation>(
        position_of(params[OperationSwitch + channel]));
  }

  void output(int channel, float voltage) {
    outputs[FuncOutput + channel].setVoltage(voltage);
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
  std::vector<rack::engine::Input> const &inputs;
  std::vector<rack::engine::Param> const &params;
  std::vector<rack::engine::Output> &outputs;
};
} // namespace func
} // namespace dhe
