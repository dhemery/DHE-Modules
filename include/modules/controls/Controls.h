#pragma once
#include "Inputs.h"
#include "modules/components/Taper.h"
#include "util/Range.h"

#include <array>
#include <engine/Module.hpp>
#include <functional>
#include <string>

namespace dhe {

static auto constexpr attenuatorRange = Range{0.F, 1.F};
static auto constexpr attenuverterRange = Range{-1.F, 1.F};
static auto constexpr gainRange = Range{-2.F, 2.F};

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

/**
 * Creates a function that returns the state of a button.
 */
auto buttonStateFunction(rack::engine::Module *module, int buttonId) -> std::function<bool()>;

/**
 * Creates a function that returns whether an input is active.
 */
auto inputIsConnectedFunction(rack::engine::Module *module, int inputId) -> std::function<bool()>;

/**
 * Creates a function that returns whether an input is high (above 1 volt).
 */
auto inputIsHighFunction(rack::engine::Module *module, int inputId) -> std::function<bool()>;

/**
 * Creates a function that returns whether an input is high (above 1 volt) or a button is pressed.
 */
auto inputIsHighOrButtonIsPressedFunction(rack::engine::Module *module, int inputId, int buttonId)
    -> std::function<bool()>;

/**
 * Creates a function that returns the modulated rotation of a knob. The amount of modulation is determined by the
 * voltage of a CV input, multiplied by the value of an attenuverter.
 */
auto rotationFunction(rack::engine::Module *module, int knobId, int cvId, int avId) -> std::function<float()>;

/**
 * Creates a function that returns the rotation of a knob, scaled to the given range.
 */
auto scaledRotationFunction(rack::engine::Module *module, int knobId, Range const &range) -> std::function<float()>;

/**
 * Creates a function that returns the rotation of a knob, scaled to a range selected by a switch from an array of
 * ranges.
 */
template <int N>
auto scaledRotationFunction(rack::engine::Module *module, int knobId, int cvId, int switchId,
                            std::array<Range const *, N> const &ranges) -> std::function<float()> {
  return [module, knobId, cvId, switchId, ranges]() -> float {
    return scaledRotation<N>(module, knobId, cvId, switchId, ranges);
  };
}

/**
 * Creates a function that returns the rotation of a knob, scaled to a range selected by a switch from an array of
 * ranges.
 */
template <int N>
auto scaledRotationFunction(rack::engine::Module *module, int knobId, int cvId, int avId, int switchId,
                            std::array<Range const *, N> const &ranges) -> std::function<float()> {
  return [module, knobId, cvId, avId, switchId, ranges]() -> float {
    return scaledRotation<N>(module, knobId, cvId, avId, switchId, ranges);
  };
}

/**
 * Creates a function that returns the rotation of a knob, tapered and scaled to the given range.
 */
auto taperedAndScaledRotationFunction(rack::engine::Module *module, int knobId, taper::FixedTaper const &taper,
                                      Range const &range) -> std::function<float()>;

/**
 * Creates a function that returns the rotation of a knob, modulated, tapered, and scaled to the given range. The
 * amount of modulation is determined by the voltage of a CV input, multiplied by the value of an attenuverter.
 */
auto taperedAndScaledRotationFunction(rack::engine::Module *module, int knobId, int cvId, int avId,
                                      taper::FixedTaper const &taper, Range const &range) -> std::function<float()>;

template <int N>
auto taperedAndScaledRotationFunction(rack::engine::Module *module, int knobId, int cvId,
                                      taper::FixedTaper const &taper, int switchId,
                                      std::array<Range const *, N> const &ranges) -> std::function<float()> {
  return [module, knobId, cvId, &taper, switchId, ranges]() -> float {
    return taperedAndScaledRotation<N>(module, knobId, cvId, taper, switchId, ranges);
  };
}

/**
 * Creates a function that returns the item selected by a switch from a given array.
 */
template <typename T, int N>
auto selectorFunction(rack::engine::Module *module, int switchId, std::array<T, N> const &items) -> std::function<T()> {
  return [module, switchId, items]() -> T { return selected<T, N>(module, switchId, items); };
}

/**
 * Creates a function that returns the range selected by a switch.
 */
template <int N>
auto rangeSelectorFunction(rack::engine::Module *module, int switchId, std::array<Range const *, N> const &ranges)
    -> std::function<Range const *()> {
  return selectorFunction<Range const *, N>(module, switchId, ranges);
}
} // namespace dhe
