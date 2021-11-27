#pragma once

#include "components/range.h"
#include "shapes.h"

#include <array>

namespace dhe {
enum class DurationRangeId { Short, Medium, Long };

namespace duration {
/*
 * Each duration range is of the form [n, 1000n]. Given ranges of that form,
 * this curvature tapers the rotation so a knob positioned dead center yields
 * a duration equal to 1/10 of the range's upper bound (to within 7 decimal
 * places).
 */

static auto constexpr taper_curvature = 0.8018017F;

static inline auto scale(float rotation, Range range) -> float {
  auto tapered = JShape::apply(rotation, taper_curvature);
  return range.scale(tapered);
}

static inline auto normalize(float seconds, Range range) -> float {
  auto const tapered = range.normalize(seconds);
  return JShape::invert(tapered, taper_curvature);
}

static auto constexpr short_range = Range{0.001F, 1.F};
static auto constexpr medium_range = Range{0.01F, 10.F};
static auto constexpr long_range = Range{0.1F, 100.F};

static auto constexpr ranges =
    std::array<Range, 3>{short_range, medium_range, long_range};

static auto constexpr labels = std::array<char const *, ranges.size()>{
    "0.001–1.0 s", "0.01–10.0 s", "0.1–100.0 s"};

template <typename D> struct Quantity {
  static inline auto scale(float normalized) -> float {
    return duration::scale(normalized, D::range);
  }

  static inline auto normalize(float scaled) -> float {
    return duration::normalize(scaled, D::range);
  }
};

} // namespace duration

struct ShortDuration : duration::Quantity<ShortDuration> {
  static auto constexpr &range = duration::short_range;
};

struct MediumDuration : duration::Quantity<MediumDuration> {
  static auto constexpr &range = duration::medium_range;
};

struct LongDuration : duration::Quantity<LongDuration> {
  static auto constexpr &range = duration::long_range;
};

struct Duration {
  static inline auto label(DurationRangeId id) -> char const * {
    return duration::labels[static_cast<size_t>(id)];
  }

  static inline auto range(DurationRangeId id) -> Range {
    return duration::ranges[static_cast<size_t>(id)];
  }

  static inline auto scale(float normalized, DurationRangeId range_id)
      -> float {
    return duration::scale(normalized, range(range_id));
  }

  static inline auto normalize(float scaled, DurationRangeId range_id)
      -> float {
    return duration::normalize(scaled, range(range_id));
  }
};

} // namespace dhe
