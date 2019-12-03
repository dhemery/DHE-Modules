#pragma once

#include "CommonInputs.h"
#include "components/Range.h"

#include <array>

namespace dhe {

/**
 * This curvature creates a gentle inverted S taper, increasing sensitivity in the middle of the knob rotation and
 * decreasing sensitivity toward the extremes.
 */
static auto constexpr durationKnobTaperCurvature = 0.8018017F;

/**
 * Each duration range is of the form [n, 1000n]. Given ranges of that form,
 * this curvature tapers the rotation so a knob positioned dead center
 * yields a duration equal to 1/10 of the range's upper bound (to within 7
 * decimal places).
 */
static auto constexpr durationKnobTaper = taper::FixedJTaper{durationKnobTaperCurvature};

auto constexpr shortDurationRange = Range{0.001F, 1.F};
auto constexpr mediumDurationRange = Range{0.01F, 10.F};
auto constexpr longDurationRange = Range{0.1F, 100.F};

static auto const durationRanges
    = std::array<Range const *, 3>{&shortDurationRange, &mediumDurationRange, &longDurationRange};

static inline auto duration(float rotation, Range const &range) -> float {
  return taperedAndScaledRotation(rotation, durationKnobTaper, range);
}

template <typename KnobType> auto duration(KnobType &knob, Range const &range) -> float {
  return duration(rotationOf(knob), range);
}

template <typename KnobType, typename ToggleType> auto selectableDuration(KnobType &knob, ToggleType &toggle) -> float {
  auto const range = selectedRange<3>(toggle, durationRanges);
  return duration(rotationOf(knob), *range);
}

template <typename KnobType, typename InputType, typename ToggleType>
auto selectableDuration(KnobType &knob, InputType &cvInput, ToggleType &toggle) -> float {
  auto const range = selectedRange<3>(toggle, durationRanges);
  return duration(rotation(knob, cvInput), *range);
}

} // namespace dhe
