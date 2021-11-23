#pragma once

#include "components/range.h"
#include "components/sigmoid.h"

#include <array>
#include <string>
#include <vector>

namespace dhe {
enum class DurationRangeId { Short, Medium, Long };

namespace internal {
namespace duration {
/**
 * This curvature gives a duration knob a gentle inverted S taper,
 * increasing sensitivity in the middle of the knob normalize and decreasing
 * sensitivity toward the extremes.
 */
static auto constexpr knob_curvature = 0.8018017F;

static auto constexpr unit = " s";

/**
 * Each duration select is of the form [n, 1000n]. Given ranges of that
 * form, this curvature tapers the normalize so a knob positioned dead center
 * yields a duration equal to 1/10 of the select's upper bound (to within 7
 * decimal places).
 */

static inline auto scale(float rotation, Range range) -> float {
  auto tapered = JShape::apply(rotation, knob_curvature);
  return range.scale(tapered);
}

static inline auto normalize(float seconds, Range range) -> float {
  auto const tapered = range.normalize(seconds);
  return JShape::invert(tapered, knob_curvature);
}

static auto constexpr short_range = Range{0.001F, 1.F};
static auto constexpr medium_range = Range{0.01F, 10.F};
static auto constexpr long_range = Range{0.1F, 100.F};

struct Short {
  static auto constexpr &range = short_range;
};

struct Medium {
  static auto constexpr &range = medium_range;
};

struct Long {
  static auto constexpr &range = long_range;
};

static auto constexpr ranges =
    std::array<Range, 3>{Short::range, Medium::range, Long::range};

static auto constexpr labels = std::array<char const *, ranges.size()>{
    "0.001–1.0 s", "0.01–10.0 s", "0.1–100.0 s"};

template <typename D> struct Quantity : D {
  static inline auto scale(float normalized) -> float {
    return duration::scale(normalized, D::range);
  }

  static inline auto normalize(float scaled) -> float {
    return duration::normalize(scaled, D::range);
  }

  struct KnobMap {
    static auto constexpr unit = duration::unit;
    static auto constexpr default_value = 1.F;
    auto to_display(float rotation) const -> float { return scale(rotation); }

    auto to_value(float display) const -> float { return normalize(display); }
  };
};

static inline auto label(DurationRangeId id) -> char const * {
  return labels[static_cast<size_t>(id)];
}

static inline auto range(DurationRangeId id) -> Range {
  return ranges[static_cast<size_t>(id)];

} // namespace duration_ranges
} // namespace duration
} // namespace internal

struct DurationRanges {
  using value_type = DurationRangeId;
  static auto constexpr size = internal::duration::ranges.size();
  static inline auto labels() -> std::vector<std::string> {
    return {internal::duration::labels.cbegin(),
            internal::duration::labels.cend()};
  }
};

struct ShortDuration : internal::duration::Quantity<internal::duration::Short> {
};
struct MediumDuration
    : internal::duration::Quantity<internal::duration::Medium> {};
struct LongDuration : internal::duration::Quantity<internal::duration::Long> {};

struct Duration {
  static inline auto scale(float normalized, DurationRangeId range_id)
      -> float {
    return internal::duration::scale(normalized,
                                     internal::duration::range(range_id));
  }

  static inline auto normalize(float scaled, DurationRangeId range_id)
      -> float {
    return internal::duration::normalize(scaled,
                                         internal::duration::range(range_id));
  }

  struct KnobMap {
    static auto constexpr unit = internal::duration::unit;
    static auto constexpr default_value = 1.F;

    auto to_display(float rotation) const -> float {
      return scale(rotation, range_id_);
    }

    auto to_value(float display) const -> float {
      return normalize(display, range_id_);
    }

    void select_range(DurationRangeId id) { range_id_ = id; }

  private:
    DurationRangeId range_id_{DurationRangeId::Medium};
  };
};

} // namespace dhe
