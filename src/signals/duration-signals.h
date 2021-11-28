#pragma once

#include "components/range.h"
#include "shape-signals.h"

#include <array>

namespace dhe {
enum class DurationRangeId { Short, Medium, Long };

namespace duration {
static auto constexpr short_range = Range{0.001F, 1.F};
static auto constexpr medium_range = Range{0.01F, 10.F};
static auto constexpr long_range = Range{0.1F, 100.F};

/*
 * Each duration range is of the form [n, 1000n]. Given ranges of that form,
 * this curvature tapers the rotation so a knob positioned dead center yields
 * a duration equal to 1/10 of the range's upper bound (to within 7 decimal
 * places).
 */
static auto constexpr taper_curvature = 0.8018017F;

static constexpr auto scale(float rotation, Range range) -> float {
  return range.scale(JShape::apply(rotation, taper_curvature));
}

static constexpr auto normalize(float seconds, Range range) -> float {
  return JShape::invert(range.normalize(seconds), taper_curvature);
}

static auto constexpr ranges =
    std::array<Range, 3>{short_range, medium_range, long_range};

static auto constexpr labels = std::array<char const *, ranges.size()>{
    "0.001–1.0 s", "0.01–10.0 s", "0.1–100.0 s"};

template <typename R> struct Tapered {
  static constexpr auto scale(float normalized) -> float {
    return duration::scale(normalized, R::range);
  }

  static constexpr auto normalize(float scaled) -> float {
    return duration::normalize(scaled, R::range);
  }
};

} // namespace duration

struct ShortDuration : duration::Tapered<ShortDuration> {
  static auto constexpr &range = duration::short_range;
};

struct MediumDuration : duration::Tapered<MediumDuration> {
  static auto constexpr &range = duration::medium_range;
};

struct LongDuration : duration::Tapered<LongDuration> {
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
