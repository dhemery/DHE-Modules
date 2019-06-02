#pragma once

#include "Inputs.h"

#include <engine/Module.hpp>

namespace dhe {
/**
 * This curvature creates a gentle inverted S taper, increasing sensitivity in the middle of the knob rotation and
 * decreasing sensitivity toward the extremes.
 */
static auto constexpr curvatureKnobTaperCurvature = -0.65F;

/**
 * Configures the param and display for a curvature knob.
 */
void configCurvatureKnob(rack::engine::Module const *module, int knobId, std::string const &name = "Curvature",
                         float initialRotation = centeredRotation);

/**
 * Configures the param and display for a curve shape switch.
 */
void configShapeSwitch(rack::engine::Module const *module, int switchId, std::string const &name = "Shape",
                       int initialState = 0);

static inline auto curvature(float rotation) -> float {
  auto const sigmoidScaledRotation = sigmoid::range.scale(rotation);
  auto const sigmoidClampedRotation = sigmoid::range.clamp(sigmoidScaledRotation);
  auto const curvature = sigmoid::curve(sigmoidClampedRotation, curvatureKnobTaperCurvature);
  return curvature;
}

static inline auto curvature(rack::engine::Module const *module, int knobId) -> float {
  return curvature(rotation(module, knobId));
}

static inline auto curvature(rack::engine::Module const *module, int knobId, int cvId) -> float {
  return curvature(rotation(module, knobId, cvId));
}

static inline auto curvature(rack::engine::Module const *module, int knobId, int cvId, int avId) -> float {
  return curvature(rotation(module, knobId, cvId, avId));
}
} // namespace dhe
