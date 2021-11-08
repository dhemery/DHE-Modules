#pragma once

#include "./common-inputs.h"

namespace dhe {
/**
 * This curvature creates a gentle inverted S taper, increasing sensitivity in
 * the middle of the knob rotation and decreasing sensitivity toward the
 * extremes.
 */
static auto constexpr curvature_knob_taper_curvature = -0.65F;

static constexpr auto curvature(float rotation) -> float {
  return sigmoid::safe_curvature_range.clamp(sigmoid::curve(
      sigmoid::domain.scale(rotation), curvature_knob_taper_curvature));
}

template <typename TParam> auto curvature(TParam &knob) -> float {
  return curvature(rotation_of(knob));
}

template <typename TParam, typename TInput>
auto curvature(TParam &knob, TInput &cv_input) -> float {
  return curvature(rotation(knob, cv_input));
}

template <typename TParam, typename TInput>
auto curvature(TParam &knob, TInput &cv_input, TParam &av_knob) -> float {
  return curvature(rotation(knob, cv_input, av_knob));
}

} // namespace dhe
