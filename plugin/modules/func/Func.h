#pragma once

#include "FuncControls.h"
#include "controls/CommonInputs.h"

#include <array>
#include <engine/Module.hpp>
#include <string>

namespace dhe {

class Range;

namespace func {

  static const auto minusTwoToPlusTwoRange = Range{-2.F, 2.F};
  static const auto minusTenToPlusTenRange = Range{-10.F, 10.F};
  static const auto zeroToFiveRange = Range{0.F, 5.F};

  std::array<Range const *, 4> const offsetRanges{&zeroToFiveRange, &bipolarSignalRange, &unipolarSignalRange,
                                                  &minusTenToPlusTenRange};

  std::array<Range const *, 4> const multiplierRanges{&attenuatorRange, &attenuverterRange, &gainRange,
                                                      &minusTwoToPlusTwoRange};

  template <int N> class Func {
  public:
    Func(std::vector<rack::engine::Input> &inputs, std::vector<rack::engine::Output> &outputs,
         std::vector<rack::engine::Param> &params) :
        inputs{inputs},
        outputs{outputs},
        params{params} {}

    auto apply(int channel, float upstream) const -> float {
      auto const in = inputs[FuncControls<N>::FuncInput + channel].getNormalVoltage(upstream);
      auto const voltage = isMultiplication(channel) ? multiply(channel, in) : add(channel, in);
      outputs[FuncControls<N>::FuncOutput + channel].setVoltage(voltage);
      return voltage;
    }

  private:
    std::vector<rack::engine::Input> &inputs;
    std::vector<rack::engine::Output> &outputs;
    std::vector<rack::engine::Param> &params;

    auto add(int channel, float in) const -> float { return in + offsetRange(channel)->scale(operand(channel)); }

    auto isMultiplication(int channel) const -> bool {
      return positionOf(params[FuncControls<N>::OperationSwitch + channel]) == 1;
    }

    auto multiplierRange(int channel) const -> Range const * {
      return multiplierRanges[positionOf(params[FuncControls<N>::MultiplierRangeSwitch + channel])];
    };

    auto multiply(int channel, float in) const -> float {
      return in * multiplierRange(channel)->scale(operand(channel));
    }

    auto offsetRange(int channel) const -> Range const * {
      return offsetRanges[positionOf(params[FuncControls<N>::OffsetRangeSwitch + channel])];
    };

    auto operand(int channel) const -> float { return rotationOf(params[FuncControls<N>::AmountKnob + channel]); }
  };
} // namespace func

} // namespace dhe
