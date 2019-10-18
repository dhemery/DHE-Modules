#pragma once

#include "CommonConfig.h"

#include <engine/Module.hpp>
#include <string>

namespace dhe {
/**
 * Configures the param and display for a curvature knob.
 */
void configCurvatureKnob(rack::engine::Module *module, int knobId, std::string const &name = "Curvature",
                         float initialRotation = centeredRotation);

/**
 * Configures the param and display for a curve shape switch.
 */
void configCurveShapeSwitch(rack::engine::Module *module, int switchId, std::string const &name = "Shape",
                            int initialState = 0);
} // namespace dhe
