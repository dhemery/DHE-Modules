#pragma once

#include "CommonInputs.h"

namespace dhe {
/**
 * This curvature creates a gentle inverted S taper, increasing sensitivity in
 * the middle of the knob rotation and decreasing sensitivity toward the
 * extremes.
 */
static auto constexpr curvature_knob_taper_curvature = -0.65F;

static inline auto curvature(float rotation) -> float {
  auto const input = sigmoid::range.scale(rotation);
  auto const curvature = sigmoid::curve(input, curvature_knob_taper_curvature);
  auto const safe_curvature = sigmoid::safe_curvature_range.clamp(curvature);
  return safe_curvature;
}

template <typename KnobType> auto curvature(KnobType &knob) -> float {
  return curvature(rotation_of(knob));
}

template <typename KnobType, typename InputType>
auto curvature(KnobType &knob, InputType &cv_input) -> float {
  return curvature(rotation(knob, cv_input));
}

template <typename KnobType, typename InputType>
auto curvature(KnobType &knob, InputType &cv_input, KnobType &av_knob)
    -> float {
  return curvature(rotation(knob, cv_input, av_knob));
}

} // namespace dhe
