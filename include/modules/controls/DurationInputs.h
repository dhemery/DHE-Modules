#pragma once

#include "CommonInputs.h"
#include "util/Range.h"

#include <array>

namespace dhe {
auto constexpr shortDurationRange = Range{0.001F, 1.F};
auto constexpr mediumDurationRange = Range{0.01F, 10.F};
auto constexpr longDurationRange = Range{0.1F, 100.F};

static auto constexpr durationKnobTaperCurvature = 0.8018017F;
/**
 * Each duration range is of the form [n, 1000n]. Given ranges of that form,
 * this curvature tapers the rotation so a knob positioned dead center
 * yields a duration equal to 1/10 of the range's upper bound (to within 7
 * decimal places).
 */
static auto constexpr durationKnobTaper = taper::FixedJTaper{durationKnobTaperCurvature};

extern std::array<Range const *, 3> const durationRanges;

/**
 * This curvature creates a gentle inverted S taper, increasing sensitivity in the middle of the knob rotation and
 * decreasing sensitivity toward the extremes.
 */

static inline auto durationRange(rack::engine::Module const *module, int switchId) -> Range const * {
  return selectedRange<3>(module, switchId, durationRanges);
}

static inline auto duration(rack::engine::Module const *module, int knobId, Range const &range) -> float {
  return taperedAndScaledRotation(module, knobId, durationKnobTaper, range);
}

static inline auto duration(rack::engine::Module const *module, int knobId, int cvId, int switchId) -> float {
  auto const range = durationRange(module, switchId);
  return taperedAndScaledRotation(module, knobId, cvId, durationKnobTaper, *range);
}

} // namespace dhe
