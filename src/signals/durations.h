#pragma once

#include "components/range.h"
#include "components/sigmoid.h"

#include <string>
#include <vector>

namespace dhe {

struct Duration {
  /**
   * This curvature creates a gentle inverted S taper, increasing sensitivity in
   * the middle of the knob rotation and decreasing sensitivity toward the
   * extremes.
   */
  static auto constexpr taper_curvature = 0.8018017F;

  /**
   * Each duration range is of the form [n, 1000n]. Given ranges of that form,
   * this curvature tapers the rotation so a knob positioned dead center
   * yields a duration equal to 1/10 of the range's upper bound (to within 7
   * decimal places).
   */
  static inline auto taper() -> sigmoid::Taper const & {
    static auto const taper = sigmoid::j_taper_with_curvature(taper_curvature);
    return taper;
  }

  static auto constexpr unit = " s";
};

template <typename TDuration> struct DurationRange {
  static auto constexpr display_range() -> Range { return TDuration::range(); }
  static auto constexpr unit = Duration::unit;

  static inline auto value(float rotation) -> float {
    return display_range().scale(Duration::taper().apply(rotation));
  }
};

struct ShortDuration : public DurationRange<ShortDuration> {
  static auto constexpr range() -> Range { return Range{0.001F, 1.F}; }
  static auto constexpr label = "0.001–1.0 s";
};

struct MediumDuration : public DurationRange<MediumDuration> {
  static auto constexpr range() -> Range { return Range{0.01F, 10.F}; }
  static auto constexpr label = "0.01–10.0 s";
};

struct LongDuration : public DurationRange<LongDuration> {
  static auto constexpr range() -> Range { return Range{0.1F, 100.F}; }
  static auto constexpr label = "0.1–100.0 s";
};

struct Durations {
  using ItemType = Range;
  enum { Short, Medium, Long };

  static auto constexpr unit = Duration::unit;

  static inline auto items() -> std::vector<Range> const & {
    static auto const items = std::vector<Range>{
        ShortDuration::range(), MediumDuration::range(), LongDuration::range()};
    return items;
  }

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        ShortDuration::label, MediumDuration::label, LongDuration::label};
    return labels;
  }

  static inline auto value(float rotation, int index) -> float {
    return items()[index].scale(Duration::taper().apply(rotation));
  }
};

} // namespace dhe
