#pragma once

#include "operation.h"

namespace dhe {

class Range;

namespace func {
template <template <int> class Controls, int N> class FuncEngine {
public:
  FuncEngine(Controls<N> &controls) : controls_{controls} {}

  auto apply(int channel, float upstream) const -> float {
    auto const input = controls_.input(channel, upstream);
    auto const is_multiply =
        controls_.operation(channel) == Operation::Multiply;
    auto const result =
        is_multiply ? multiply(channel, input) : add(channel, input);
    controls_.output(channel, result);
    return result;
  }

private:
  auto add(int channel, float augend) const -> float {
    auto const addend_range = controls_.offset_range(channel);
    auto const addend_rotation = controls_.operand(channel);
    auto const addend = addend_range->scale(addend_rotation);
    return augend + addend;
  }

  auto multiply(int channel, float multiplicand) const -> float {
    auto const multiplier_range = controls_.multiplier_range(channel);
    auto const multiplier_rotation = controls_.operand(channel);
    auto const multiplier = multiplier_range->scale(multiplier_rotation);
    return multiplicand * multiplier;
  }

  Controls<N> &controls_;
};
} // namespace func

} // namespace dhe
