#pragma once

#include "signals/gain.h"

#include <array>

namespace dhe {
namespace func {

enum class MultiplierRangeId { Attenuator, Attenuverter, Gain, Gainuverter };

namespace multiplier_ranges {
static auto constexpr size = 4;
static auto constexpr labels =
    std::array<char const *, size>{"0–1", "±1", "0–2", "±2"};

static auto constexpr ranges = std::array<Range, size>{
    Attenuator::range, Attenuverter::range, Gain::range, {-2.F, 2.F}};

static inline auto range(MultiplierRangeId id) -> Range {
  return ranges[static_cast<size_t>(id)];
}

static inline auto scale(float normalized, MultiplierRangeId range_id)
    -> float {
  return range(range_id).scale(normalized);
}

static inline auto normalize(float scaled, MultiplierRangeId range_id)
    -> float {
  return range(range_id).normalize(scaled);
}

} // namespace multiplier_ranges

struct MultiplierRanges {
  using ValueType = MultiplierRangeId;
  static auto constexpr size = multiplier_ranges::size;
  static auto constexpr &labels = multiplier_ranges::labels;
  static auto constexpr unit = "";
  static auto constexpr stepper_slug = "multiplier-range";
};

} // namespace func
} // namespace dhe
