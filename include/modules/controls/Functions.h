#pragma once
#include "Inputs.h"
#include "modules/components/Taper.h"
#include "util/Range.h"

#include <array>
#include <engine/Module.hpp>
#include <functional>
#include <string>

namespace dhe {

/**
 * Creates a function that returns the state of a button.
 */
static inline auto buttonIsPressedFunction(rack::engine::Module const *module, int buttonId) -> std::function<bool()> {
  return [module, buttonId]() -> bool { return buttonIsPressed(module, buttonId); };
}

/**
 * Creates a function that returns whether an input is active.
 */
static inline auto inputIsConnectedFunction(rack::engine::Module const *module, int inputId) -> std::function<bool()> {
  return [module, inputId]() -> bool { return inputIsConnected(module, inputId); };
}

/**
 * Creates a function that returns whether an input is high (above 1 volt).
 */
static inline auto inputIsHighFunction(rack::engine::Module const *module, int inputId) -> std::function<bool()> {
  return [module, inputId]() -> bool { return inputIsHigh(module, inputId); };
}

/**
 * Creates a function that returns whether an input is high (above 1 volt) or a button is pressed.
 */
static inline auto inputIsHighOrButtonIsPressedFunction(rack::engine::Module const *module, int inputId, int buttonId)
    -> std::function<bool()> {
  return [module, inputId, buttonId]() -> bool {
    return inputIsHigh(module, inputId) || buttonIsPressed(module, buttonId);
  };
}

/**
 * Creates a function that returns the rotation of a knob, tapered and scaled to the given range.
 */
static inline auto taperedAndScaledRotationFunction(rack::engine::Module const *module, int knobId,
                                                    taper::FixedTaper const &taper, Range const &range)
    -> std::function<float()> {
  return [module, knobId, &taper, &range]() -> float { return taperedAndScaledRotation(module, knobId, taper, range); };
}

template <int N>
auto taperedAndScaledRotationFunction(rack::engine::Module const *module, int knobId, int cvId,
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
auto selectorFunction(rack::engine::Module const *module, int switchId, std::array<T, N> const &items)
    -> std::function<T()> {
  return [module, switchId, items]() -> T { return selected<T, N>(module, switchId, items); };
}

/**
 * Creates a function that returns the range selected by a switch.
 */
template <int N>
auto rangeSelectorFunction(rack::engine::Module const *module, int switchId, std::array<Range const *, N> const &ranges)
    -> std::function<Range const *()> {
  return selectorFunction<Range const *, N>(module, switchId, ranges);
}
} // namespace dhe
