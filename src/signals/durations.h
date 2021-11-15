#pragma once

#include "components/range.h"
#include "components/sigmoid.h"
#include "enums.h"

#include <string>
#include <vector>

namespace dhe {
namespace duration {
static inline auto taper() -> sigmoid::Taper const & {
  /**
   * This curvature gives a duration knob a gentle inverted S taper,
   * increasing sensitivity in the middle of the knob normalize and decreasing
   * sensitivity toward the extremes.
   */
  static auto constexpr taper_curvature = 0.8018017F;

  /**
   * Each duration select is of the form [n, 1000n]. Given ranges of that
   * form, this curvature tapers the normalize so a knob positioned dead center
   * yields a duration equal to 1/10 of the select's upper bound (to within 7
   * decimal places).
   */
  static auto constexpr taper =
      sigmoid::j_taper_with_curvature(taper_curvature);
  return taper;
}

static inline auto scale(float rotation, Range range) -> float {
  auto tapered = taper().apply(rotation);
  return range.scale(tapered);
}

static inline auto normalize(float seconds, Range range) -> float {
  auto const tapered = range.normalize(seconds);
  return taper().invert(tapered);
}
} // namespace duration

static auto constexpr duration_unit = " s";

template <typename T> struct MappedDurationRange {
  static auto constexpr max = T::max;
  static auto constexpr min = max / 1000.F;
  static auto constexpr default_value = max / 10.F;
  struct KnobMap;

  static constexpr auto range() -> Range { return Range{min, max}; }

  static inline auto scale(float normalized) -> float {
    return duration::scale(normalized, range());
  }

  static inline auto normalize(float scaled) -> float {
    return duration::normalize(scaled, range());
  }
};

template <typename T> struct MappedDurationRange<T>::KnobMap {
  static auto constexpr default_value = T::max / 10.F;
  static auto constexpr unit = duration_unit;

  auto to_display(float value) const -> float {
    return MappedDurationRange<T>::scale(value);
  }

  auto to_value(float display) const -> float {
    return MappedDurationRange<T>::normalize(display);
  }
};

struct ShortDurationBounds {
  static auto constexpr max = 1.F;
};
struct MediumDurationBounds {
  static auto constexpr max = 10.F;
};
struct LongDurationBounds {
  static auto constexpr max = 100.F;
};

struct ShortDuration : MappedDurationRange<ShortDurationBounds> {
  static auto constexpr label = "0.001–1.0 s";
};

struct MediumDuration : MappedDurationRange<MediumDurationBounds> {
  static auto constexpr label = "0.01–10.0 s";
};

struct LongDuration : MappedDurationRange<LongDurationBounds> {
  static auto constexpr label = "0.1–100.0 s";
};

enum class DurationRangeId { Short, Medium, Long };

struct DurationRanges : Enums<DurationRangeId, 3> {
  struct KnobMap;

  // TODO: Move to SwitchMap
  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        ShortDuration::label, MediumDuration::label, LongDuration::label};
    return labels;
  }

  static inline auto scale(float normalized, DurationRangeId range_id)
      -> float {
    return duration::scale(normalized, range(range_id));
  }

  static inline auto normalize(float scaled, DurationRangeId range_id)
      -> float {
    return duration::normalize(scaled, range(range_id));
  }

  static inline auto range(DurationRangeId id) -> Range {
    return ranges()[static_cast<int>(id)];
  }

  static inline auto ranges() -> std::vector<Range> const & {
    static auto const ranges = std::vector<Range>{
        ShortDuration::range(), MediumDuration::range(), LongDuration::range()};
    return ranges;
  }
};

struct DurationRanges::KnobMap {
  static auto constexpr unit = duration_unit;
  static auto constexpr default_value =
      MediumDuration::default_value; // Centere

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

} // namespace dhe
