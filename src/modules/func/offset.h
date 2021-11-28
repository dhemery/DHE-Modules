#pragma once

#include "components/range.h"
#include "signals/voltage-signals.h"

#include <array>

namespace dhe {
namespace func {

enum class OffsetRangeId { Unipolar5, Bipolar, Unipolar, Bipolar10 };

namespace offsets {
static auto constexpr ranges =
    std::array<Range, 4>{Range{0.F, 5.F}, BipolarVoltage::range,
                         UnipolarVoltage::range, Range{-10.F, 10.F}};
static auto constexpr labels =
    std::array<char const *, ranges.size()>{"0–5 V", "±5 V", "0–10 V", "±10 V"};

} // namespace offsets

struct Offset {
  static auto constexpr &ranges = offsets::ranges;

  static inline auto range(OffsetRangeId id) -> Range {
    return ranges[static_cast<size_t>(id)];
  }

  static inline auto scale(float normalized, OffsetRangeId range_id) -> float {
    return range(range_id).scale(normalized);
  }

  static inline auto normalize(float scaled, OffsetRangeId range_id) -> float {
    return range(range_id).normalize(scaled);
  }
};
} // namespace func
} // namespace dhe
