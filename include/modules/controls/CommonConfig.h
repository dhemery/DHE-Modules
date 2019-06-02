#pragma once

#include "util/Range.h"

#include <engine/Module.hpp>
#include <string>

namespace dhe {
static auto constexpr centeredRotation = 0.5F;

/**
 * Configures the param and display for an attenuator knob.
 */
void configAttenuator(rack::engine::Module *module, int knobId, std::string const &knobName);

/**
 * Configures the param and display for an attenuverter knob.
 */
void configAttenuverter(rack::engine::Module *module, int knobId, std::string const &knobName);

/**
 * Configures the param and display for a momentary button.
 */
void configButton(rack::engine::Module *module, int buttonId, std::string const &buttonName,
                  std::array<std::string, 2> const &stateNames, int initialState);

/**
 * Configures the param and display for a gain knob.
 */
void configGain(rack::engine::Module *module, int knobId, std::string const &knobName);

/**
 * Configures the param and display for a knob with a fixed, linear range.
 */
void configKnob(rack::engine::Module *module, int knobId, std::string const &knobName, std::string const &units,
                Range const &range, float initialRotation = centeredRotation);

/**
 * Configures the param and display for a knob with a fixed, linear range, displayed as a percentage.
 */
void configPercentageKnob(rack::engine::Module *module, int knobId, std::string const &knobName,
                          Range const &range = {0.F, 1.F});

/**
 * Configures the param and display for a toggle that represents a sequence of states.
 */
template <int N>
void configToggle(rack::engine::Module *module, int toggleId, std::string const &toggleName,
                  std::array<std::string, N> const &stateNames, int initialState);

} // namespace dhe
