#pragma once

#include "CommonInputs.h"
#include "util/Range.h"

#include <array>

namespace dhe {
auto constexpr shortDurationRange = Range{0.001F, 1.F};
auto constexpr mediumDurationRange = Range{0.01F, 10.F};
auto constexpr longDurationRange = Range{0.1F, 100.F};
static auto constexpr durationKnobTaperCurvature = 0.8018017F;

extern std::array<Range const *, 3> const durationRanges;

/**
 * This curvature creates a gentle inverted S taper, increasing sensitivity in the middle of the knob rotation and
 * decreasing sensitivity toward the extremes.
 */

static inline auto durationRange(rack::engine::Module const *module, int switchId) -> Range const * {
  return selectedRange<3>(module, switchId, durationRanges);
}

static inline auto duration(float rotation, Range const *range) -> float {
  auto const sigmoidScaledRotation = sigmoid::range.scale(rotation);
  auto const sigmoidClampedRotation = sigmoid::range.clamp(sigmoidScaledRotation);
  auto const taperedRotation = sigmoid::curve(sigmoidClampedRotation, durationKnobTaperCurvature);
  return range->scale(taperedRotation);
}

static inline auto duration(rack::engine::Module const *module, int knobId, Range const &range) -> float {
  return duration(rotation(module, knobId), &range);
}

static inline auto selectableDuration(rack::engine::Module const *module, int knobId, int cvId, int switchId) -> float {
  auto const range = durationRange(module, switchId);
  return duration(rotation(module, knobId, cvId), range);
}

} // namespace dhe
