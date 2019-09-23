#pragma once

#include "CommonInputs.h"

#include <engine/Module.hpp>

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

static inline auto curvature(rack::engine::Module const *module, int knobId) -> float {
  return curvature(paramValue(module->params[knobId]));
}

static inline auto curvature(rack::engine::Module const *module, int knobId, int cvId) -> float {
  return curvature(rotation(module->params[knobId], module->inputs[cvId]));
}

static inline auto curvature(rack::engine::Module const *module, int knobId, int cvId, int avId) -> float {
  return curvature(rotation(module->params[knobId], module->inputs[cvId], module->params[avId]));
}
} // namespace dhe
