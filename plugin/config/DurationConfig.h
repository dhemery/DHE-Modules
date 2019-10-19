#pragma once

#include "CommonConfig.h"
#include "components/Range.h"

#include <array>
#include <engine/Module.hpp>

namespace dhe {
/**
 * Configures the param and display for a duration knob with a fixed range.
 */
void configDurationKnob(rack::engine::Module *module, int knobId, Range const &range,
                        std::string const &name = "Duration", float initialRotation = centeredRotation);

/**
 * Configures the param and display for a duration knob with a range selected by a switch.
 */
void configDurationKnob(rack::engine::Module *module, int knobId, int switchId, std::string const &name = "Duration",
                        float initialRotation = centeredRotation);

/**
 * Configures the param and display for a duration range switch.
 */
void configDurationRangeSwitch(rack::engine::Module *module, int switchId, std::string const &name = "Duration Range",
                               int initialPosition = 1);
} // namespace dhe
