#pragma once

#include "components/range.h"
#include "components/sigmoid.h"

#include <string>
#include <vector>

namespace dhe {

struct DurationTaper {
  static inline auto taper() -> sigmoid::Taper const & {
    /**
     * This curvature creates a gentle inverted S taper, increasing sensitivity
     * in the middle of the knob rotation and decreasing sensitivity toward the
     * extremes.
     */
    static auto constexpr taper_curvature = 0.8018017F;

    /**
     * Each duration range is of the form [n, 1000n]. Given ranges of that form,
     * this curvature tapers the rotation so a knob positioned dead center
     * yields a duration equal to 1/10 of the range's upper bound (to within 7
     * decimal places).
     */
    static auto constexpr taper =
        sigmoid::j_taper_with_curvature(taper_curvature);
    return taper;
  }

  static inline auto tapered(float rotation) -> float {
    return taper().apply(rotation);
  }

  static inline auto rotation(float tapered) -> float {
    return taper().invert(tapered);
  }
};

// Maps a knob param value (rotation) to and from a display value (seconds) for
// a given duration range.
template <typename TDurationRange> struct DurationRangeKnobMapper {
  auto to_value(float seconds) const -> float {
    auto const tapered = TDurationRange::display_range().normalize(seconds);
    return DurationTaper::rotation(tapered);
  }

  auto to_display_value(float rotation) const -> float {
    return TDurationRange::value(rotation);
  }
};

// Maps a knob param value (rotation) to and from a display value (seconds) for
// the selected voltage range.
template <typename TDurationRanges> struct SelectableDurationRangeKnobMapper {
  auto to_value(float seconds) const -> float {
    auto const tapered =
        TDurationRanges::select(range_index_).normalize(seconds);
    return DurationTaper::rotation(tapered);
  }

  auto to_display_value(float rotation) const -> float {
    return TDurationRanges::value(rotation, range_index_);
  }

  void select_range(int range_index) { range_index_ = range_index; }

private:
  int range_index_{TDurationRanges::Medium};
};

template <typename TDuration> struct DurationRange {
  static auto constexpr display_range() -> Range { return TDuration::range(); }
  static auto constexpr unit = " s";

  static inline auto value(float rotation) -> float {
    return display_range().scale(DurationTaper::tapered(rotation));
  }

  using DisplayMapper = DurationRangeKnobMapper<TDuration>;
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
  using PositionType = int;
  using ItemType = Range;
  using DisplayMapper = SelectableDurationRangeKnobMapper<Durations>;
  enum { Short, Medium, Long };

  static auto constexpr unit = " s";

  static inline auto items() -> std::vector<ItemType> const & {
    static auto const items = std::vector<ItemType>{
        ShortDuration::range(), MediumDuration::range(), LongDuration::range()};
    return items;
  }

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        ShortDuration::label, MediumDuration::label, LongDuration::label};
    return labels;
  }

  static inline auto select(int selection) -> ItemType const & {
    return items()[selection];
  }

  static inline auto value(float rotation, int selection) -> float {
    auto const tapered = DurationTaper::tapered(rotation);
    return select(selection).scale(tapered);
  }
};

} // namespace dhe
