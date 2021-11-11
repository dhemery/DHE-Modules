#pragma once

#include "params.h"

namespace dhe {

class Range;

namespace func {
template <typename Signals> class FuncEngine {
public:
  FuncEngine(Signals &signals) : signals_{signals} {}

  auto apply(int channel, float upstream) const -> float {
    auto const input = signals_.input(channel, upstream);
    auto const is_multiply =
        signals_.operation(channel) == Operations::Multiply;
    auto const result =
        is_multiply ? multiply(channel, input) : add(channel, input);
    signals_.output(channel, result);
    return result;
  }

private:
  auto add(int channel, float augend) const -> float {
    auto const rotation = signals_.operand(channel);
    auto const range = signals_.offset_range(channel);
    auto const addend = Offsets::offset(rotation, range);
    return augend + addend;
  }

  auto multiply(int channel, float multiplicand) const -> float {
    auto const rotation = signals_.operand(channel);
    auto const range = signals_.multiplier_range(channel);
    auto const multiplier = Multipliers::multiplier(rotation, range);
    return multiplicand * multiplier;
  }

  Signals &signals_;
};
} // namespace func

} // namespace dhe
