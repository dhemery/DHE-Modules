#pragma once

#include "CommonInputs.h"
#include "modules/components/Range.h"

#include <engine/Module.hpp>
#include <string>

namespace dhe {
static auto constexpr centeredRotation = 0.5F;

/**
 * Configures the param and display for a gain knob.
 */
static inline void configKnob(rack::engine::Module *module, int knobId, std::string const &knobName,
                              std::string const &units, Range const &range, float initialRotation = centeredRotation) {
  module->configParam(knobId, 0.F, 1.F, initialRotation, knobName, units, 0.F, range.size(), range.lowerBound);
}

/**
 * Configures the param and display for a knob with a fixed, linear range, displayed as a percentage.
 */
static inline void configPercentageKnob(rack::engine::Module *module, int knobId, std::string const &knobName,
                                        Range const &range = rotationRange) {
  module->configParam(knobId, 0.F, 1.F, 0.5F, knobName, "%", 0.F, range.size() * 100.F, range.lowerBound * 100.F);
}

/**
 * Configures the param and display for an attenuator knob.
 */
static inline void configAttenuator(rack::engine::Module *module, int knobId, std::string const &knobName) {
  configPercentageKnob(module, knobId, knobName, attenuatorRange);
}

/**
 * Configures the param and display for an attenuverter knob.
 */
static inline void configAttenuverter(rack::engine::Module *module, int knobId, std::string const &knobName) {
  configPercentageKnob(module, knobId, knobName, attenuverterRange);
}

/**
 * Configures the param and display for a momentary button.
 */
static inline void configGain(rack::engine::Module *module, int knobId, std::string const &knobName) {
  configPercentageKnob(module, knobId, knobName, gainRange);
}

template <int N> class FrameWidgetParamQuantity : public rack::engine::ParamQuantity {
public:
  void setFrameNames(std::array<std::string, N> const &names) { frameNames = names; }
  auto getDisplayValueString() -> std::string override {
    auto const frame = static_cast<int>(getValue());
    return frameNames[frame];
  }

private:
  std::array<std::string, N> frameNames;
};

template <int N>
void configFrameWidgetStates(rack::engine::Module *module, int paramId, std::string const &paramName,
                             std::array<std::string, N> const &stateNames, int initialState) {
  module->configParam<FrameWidgetParamQuantity<N>>(paramId, 0.F, (float) (N - 1), (float) initialState, paramName);
  auto const controlDisplay = dynamic_cast<FrameWidgetParamQuantity<N> *>(module->paramQuantities[paramId]);
  controlDisplay->setFrameNames(stateNames);
}

/**
 * Configures the param and display for a momentary button.
 */
static inline void configButton(rack::engine::Module *module, int buttonId, std::string const &buttonName,
                                std::array<std::string, 2> const &stateNames, int initialState) {
  configFrameWidgetStates<2>(module, buttonId, buttonName, stateNames, initialState);
}

/**
 * Configures the param and display for a toggle that represents a sequence of states.
 */
template <int N>
void configToggle(rack::engine::Module *module, int toggleId, std::string const &toggleName,
                  std::array<std::string, N> const &stateNames, int initialState) {
  configFrameWidgetStates<N>(module, toggleId, toggleName, stateNames, initialState);
}

} // namespace dhe
