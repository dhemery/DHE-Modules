#pragma once

#include "CommonInputs.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>

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

static inline auto curvature(rack::engine::Param const &knob) -> float { return curvature(paramValue(knob)); }

static inline auto curvature(rack::engine::Param const &knob, rack::engine::Input const &cvInput) -> float {
  return curvature(rotation(knob, cvInput));
}

static inline auto curvature(rack::engine::Param const &knob, rack::engine::Input const &cvInput,
                             rack::engine::Param const &avKnob) -> float {
  return curvature(rotation(knob, cvInput, avKnob));
}

} // namespace dhe
