#pragma once

#include "components/range.h"
#include "signals/voltage-signals.h"

#include <array>

namespace dhe {
namespace func {

enum class OffsetRangeId { Unipolar5, Bipolar, Unipolar, Bipolar10 };

namespace offset_ranges {
static auto constexpr size = 4;
static auto constexpr ranges =
    std::array<Range, size>{Range{0.F, 5.F}, BipolarVoltage::range,
                            UnipolarVoltage::range, Range{-10.F, 10.F}};
static auto constexpr labels =
    std::array<char const *, size>{"0–5 V", "±5 V", "0–10 V", "±10 V"};

static inline auto range(OffsetRangeId id) -> Range {
  return ranges[static_cast<size_t>(id)];
}

static inline auto scale(float normalized, OffsetRangeId range_id) -> float {
  return range(range_id).scale(normalized);
}

static inline auto normalize(float scaled, OffsetRangeId range_id) -> float {
  return range(range_id).normalize(scaled);
}

} // namespace offset_ranges

struct OffsetRanges {
  using ValueType = OffsetRangeId;
  static auto constexpr &labels = offset_ranges::labels;
  static auto constexpr size = offset_ranges::size;
  static auto constexpr stepper_slug = "offset-range";
  static auto constexpr unit = voltage::unit;
};

} // namespace func
} // namespace dhe
