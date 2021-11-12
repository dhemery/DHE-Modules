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
    auto const is_multiply = signals_.operation(channel) == Operation::Multiply;
    auto const result =
        is_multiply ? multiply(channel, input) : add(channel, input);
    signals_.output(channel, result);
    return result;
  }

private:
  auto add(int channel, float augend) const -> float {
    auto const rotation = signals_.operand(channel);
    auto const range_id = signals_.offset_range(channel);
    auto const addend = OffsetRanges::scale(rotation, range_id);
    return augend + addend;
  }

  auto multiply(int channel, float multiplicand) const -> float {
    auto const rotation = signals_.operand(channel);
    auto const range_id = signals_.multiplier_range(channel);
    auto const multiplier = MultiplierRanges::scale(rotation, range_id);
    return multiplicand * multiplier;
  }

  Signals &signals_;
};
} // namespace func

} // namespace dhe
