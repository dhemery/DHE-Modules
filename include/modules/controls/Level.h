#pragma once

#include "util/range.h"

#include <engine/Module.hpp>

#include <array>

namespace dhe {
namespace level {
static float constexpr centeredRotation = 0.5;

static Range constexpr unipolarRange{0.f, 10.f};
static Range constexpr bipolarRange{-5.f, 5.f};

extern std::array<Range const *, 2> const ranges;

/**
 * Returns the range for the given range switch position.
 * @param switchPosition the position of the range switch
 */
auto range(float switchPosition) -> Range const *;

/**
 * Creates a function that yields the level (in volts) selected by a
 * cv-modulated knob and a level range switch.
 * @param knobId the ID of the level knob param
 * @param cvId the ID of the control voltage input
 * @param switchId the ID of the level range switch
 */
auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId,
                         int switchId) -> std::function<float()>;

/**
 * Creates a function that yields the level (in volts) selected by a
 * cv-modulated knob and a level range switch. The control voltage is further
 * modulated by an attenuverter.
 * @param knobId the ID of the level knob param
 * @param cvId the ID of the control voltage input
 * @param avId the ID of the attenuverter param
 * @param switchId the ID of the level range switch
 */
auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId,
                         int avId, int switchId) -> std::function<float()>;

/**
 * Creates a function that yields the level (in volts) selected by a knob
 * from a given range.
 * @param knobId the ID of the level knob param
 * @param range the range from which to select a voltage
 */
auto withFixedRange(rack::engine::Module *module, int knobId,
                    Range const &range) -> std::function<float()>;

void configKnob(rack::engine::Module *module, int knobId, Range const &range,
                std::string const &name = "Level",
                float initialPosition = centeredRotation);

void configKnob(rack::engine::Module *module, int knobId, int switchId,
                std::string const &name = "Level",
                float initialPosition = centeredRotation);

void configSwitch(rack::engine::Module *module, int switchId,
                  std::string const &name = "Level Range",
                  int initialPosition = 1);
} // namespace level
} // namespace dhe
