#pragma once

#include "Config.h"
#include "util/Range.h"

#include <engine/Module.hpp>
#include <string>

namespace dhe {
/**
 * Configures the param and display for a level knob with a fixed range.
 */
void configLevelKnob(rack::engine::Module *module, int knobId, Range const &range, std::string const &name = "Level",
                     float initialRotation = centeredRotation);

/**
 * Configures the param and display for a level knob with a range selected by a switch.
 */
void configLevelKnob(rack::engine::Module *module, int knobId, int switchId, std::string const &name = "Level",
                     float initialRotation = centeredRotation);

/**
 * Configures the param and display for a level range switch.
 */
void configLevelRangeSwitch(rack::engine::Module *module, int switchId, std::string const &name = "Level Range",
                            int initialState = 1);
} // namespace dhe
