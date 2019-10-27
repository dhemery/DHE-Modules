#pragma once

#include "CommonInputs.h"

namespace dhe {
/**
 * This curvature creates a gentle inverted S taper, increasing sensitivity in the middle of the knob rotation and
 * decreasing sensitivity toward the extremes.
 */
static auto constexpr curvatureKnobTaperCurvature = -0.65F;

static inline auto curvature(float rotation) -> float {
  auto const input = sigmoid::range.scale(rotation);
  auto const curvature = sigmoid::curve(input, curvatureKnobTaperCurvature);
  auto const safeCurvature = sigmoid::safeCurvatureRange.clamp(curvature);
  return safeCurvature;
}

template <typename KnobType> auto curvature(KnobType &knob) -> float { return curvature(rotationOf(knob)); }

template <typename KnobType, typename InputType> auto curvature(KnobType &knob, InputType &cvInput) -> float {
  return curvature(rotation(knob, cvInput));
}

template <typename KnobType, typename InputType>
auto curvature(KnobType &knob, InputType &cvInput, KnobType &avKnob) -> float {
  return curvature(rotation(knob, cvInput, avKnob));
}

} // namespace dhe
