#pragma once

#include "components/range.h"
#include "components/sigmoid.h"

#include <string>
#include <vector>

namespace dhe {

struct DurationKnobTaper {
  static auto tapered(float rotation) -> float {
    return taper().apply(rotation);
  }

  static auto rotation(float tapered) -> float {
    return taper().invert(tapered);
  }

private:
  static auto taper() -> sigmoid::Taper const & {
    /**
     * This curvature gives a duration knob a gentle inverted S taper,
     * increasing sensitivity in the middle of the knob rotation and decreasing
     * sensitivity toward the extremes.
     */
    static auto constexpr taper_curvature = 0.8018017F;

    /**
     * Each duration select is of the form [n, 1000n]. Given ranges of that
     * form, this curvature tapers the rotation so a knob positioned dead center
     * yields a duration equal to 1/10 of the select's upper bound (to within 7
     * decimal places).
     */
    static auto const taper = sigmoid::j_taper_with_curvature(taper_curvature);
    return taper;
  }
};

template <typename TDuration> struct DurationRange {
  static auto constexpr display_range() -> Range {
    return TDuration::range();
  }
  static auto constexpr unit = " s";

  static inline auto seconds(float rotation) -> float {
    auto tapered = DurationKnobTaper::tapered(rotation);
    return display_range().scale(tapered);
  }

  static inline auto rotation(float seconds) -> float {
    auto const tapered = display_range().normalize(seconds);
    return DurationKnobTaper::rotation(tapered);
  }

  struct KnobMapper {
    auto to_display_value(float rotation) const -> float {
      return seconds(rotation);
    }

    auto to_rotation(float seconds) const -> float { return rotation(seconds); }
  };
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
  enum Selection { Short, Medium, Long };

  static auto constexpr unit = " s";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        ShortDuration::label, MediumDuration::label, LongDuration::label};
    return labels;
  }

  static inline auto select(int selection) -> Range const & {
    static auto const ranges = std::vector<Range>{
        ShortDuration::range(), MediumDuration::range(), LongDuration::range()};
    return ranges[selection];
  }

  static inline auto seconds(float rotation, int selection) -> float {
    auto const tapered = DurationKnobTaper::tapered(rotation);
    return select(selection).scale(tapered);
  }

  static inline auto rotation(float seconds, int selection) -> float {
    auto const tapered = select(selection).normalize(seconds);
    return DurationKnobTaper::rotation(tapered);
  }

  struct KnobMapper {
    auto to_display_value(float rotation) const -> float {
      return seconds(rotation, range_);
    }

    auto to_rotation(float seconds) const -> float {
      return rotation(seconds, range_);
    }

    void select_range(Selection selection) { range_ = selection; }

  private:
    Selection range_{Medium};
  };
};

} // namespace dhe
