#pragma once

#include "components/range.h"
#include "components/sigmoid.h"

#include <string>
#include <vector>

namespace dhe {
namespace duration {
static auto constexpr unit = " s";

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
  static auto const taper = sigmoid::j_taper_with_curvature(taper_curvature);
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

template <typename T> struct MappedDurationRange {
  struct KnobMapper;
  static auto constexpr unit = duration::unit;

  static inline auto scale(float normalized) -> float {
    return duration::scale(normalized, T::range());
  }

  static inline auto normalize(float scaled) -> float {
    return duration::normalize(scaled, T::range());
  }
};

template <typename T> struct MappedDurationRange<T>::KnobMapper {
  auto scale(float normalized) const -> float {
    return MappedDurationRange<T>::scale(normalized);
  }

  auto normalize(float scaled) const -> float {
    return MappedDurationRange<T>::normalize(scaled);
  }
};

struct ShortDuration : MappedDurationRange<ShortDuration> {
  static auto constexpr range() -> Range { return Range{0.001F, 1.F}; }
  static auto constexpr label = "0.001–1.0 s";
};

struct MediumDuration : MappedDurationRange<MediumDuration> {
  static auto constexpr range() -> Range { return Range{0.01F, 10.F}; }
  static auto constexpr label = "0.01–10.0 s";
};

struct LongDuration : MappedDurationRange<LongDuration> {
  static auto constexpr range() -> Range { return Range{0.1F, 100.F}; }
  static auto constexpr label = "0.1–100.0 s";
};

enum class DurationRangeId { Short, Medium, Long };

struct DurationRanges {
  struct KnobMapper;
  using Selection = DurationRangeId;
  static auto constexpr unit = ShortDuration::unit;

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

  static inline auto range(DurationRangeId range_id) -> Range const & {
    static auto const ranges = std::vector<Range>{
        ShortDuration::range(), MediumDuration::range(), LongDuration::range()};
    return ranges[(int)range_id];
  }
};

struct DurationRanges::KnobMapper {
  auto scale(float normalized) const -> float {
    return DurationRanges::scale(normalized, range_id_);
  }

  auto normalize(float scaled) const -> float {
    return DurationRanges::normalize(scaled, range_id_);
  }

  void select_range(DurationRangeId id) { range_id_ = id; }

private:
  DurationRangeId range_id_{};
};

} // namespace dhe