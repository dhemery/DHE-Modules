#pragma once

#include "Operation.h"

namespace dhe {

class Range;

namespace func {
  template <template <int> class Controls, int N> class FuncEngine {
  public:
    FuncEngine(Controls<N> &controls) : controls{controls} {}

    auto apply(int channel, float upstream) const -> float {
      auto const input = controls.input(channel, upstream);
      auto const isMultiply = controls.operation(channel) == Operation::Multiply;
      auto const result = isMultiply ? multiply(channel, input) : add(channel, input);
      controls.output(channel, result);
      return result;
    }

  private:
    auto add(int channel, float augend) const -> float {
      auto const addendRange = controls.offsetRange(channel);
      auto const addendRotation = controls.operand(channel);
      auto const addend = addendRange->scale(addendRotation);
      return augend + addend;
    }

    auto multiply(int channel, float multiplicand) const -> float {
      auto const multiplierRange = controls.multiplierRange(channel);
      auto const multiplierRotation = controls.operand(channel);
      auto const multiplier = multiplierRange->scale(multiplierRotation);
      return multiplicand * multiplier;
    }

    Controls<N> &controls;
  };
} // namespace func

} // namespace dhe
