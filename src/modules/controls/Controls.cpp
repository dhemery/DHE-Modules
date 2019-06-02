#include "modules/controls/Controls.h"

#include "util/Range.h"

namespace dhe {

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Local functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////

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

/**
 * Configures the display for a frame widget that represents a sequence of states.
 */
template <int N>
void configFrameWidgetStates(rack::engine::Module const *module, int paramId, std::string const &paramName,
                             std::array<std::string, N> const &stateNames, int initialState) {
  nonConst(module)->configParam<FrameWidgetParamQuantity<N>>(paramId, 0.F, (float) (N - 1), (float) initialState,
                                                             paramName);
  auto const controlDisplay = dynamic_cast<FrameWidgetParamQuantity<N> *>(module->paramQuantities[paramId]);
  controlDisplay->setFrameNames(stateNames);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void configAttenuator(rack::engine::Module const *module, int knobId, std::string const &knobName) {
  configPercentageKnob(module, knobId, knobName, attenuatorRange);
}

void configAttenuverter(rack::engine::Module const *module, int knobId, std::string const &knobName) {
  configPercentageKnob(module, knobId, knobName, attenuverterRange);
}

void configButton(rack::engine::Module const *module, int buttonId, std::string const &buttonName,
                  std::array<std::string, 2> const &stateNames, int initialState) {
  configFrameWidgetStates<2>(module, buttonId, buttonName, stateNames, initialState);
}

void configGain(rack::engine::Module const *module, int knobId, std::string const &knobName) {
  configPercentageKnob(module, knobId, knobName, gainRange);
}

void configKnob(rack::engine::Module const *module, int knobId, std::string const &knobName, std::string const &units,
                Range const &range, float initialRotation) {
  nonConst(module)->configParam(knobId, 0.F, 1.F, initialRotation, knobName, units, 0.F, range.size(),
                                range.lowerBound);
}

void configPercentageKnob(rack::engine::Module const *module, int knobId, std::string const &knobName,
                          Range const &range) {
  nonConst(module)->configParam(knobId, 0.F, 1.F, 0.5F, knobName, "%", 0.F, range.size() * 100.F,
                                range.lowerBound * 100.F);
}

template <int N>
void configToggle(rack::engine::Module const *module, int toggleId, std::string const &toggleName,
                  std::array<std::string, N> const &stateNames, int initialState) {
  configFrameWidgetStates<N>(module, toggleId, toggleName, stateNames, initialState);
}

auto buttonIsPressedFunction(rack::engine::Module const *module, int buttonId) -> std::function<bool()> {
  return [module, buttonId]() -> bool { return buttonIsPressed(module, buttonId); };
}

auto inputIsConnectedFunction(rack::engine::Module const *module, int inputId) -> std::function<bool()> {
  return [module, inputId]() -> bool { return inputIsConnected(module, inputId); };
}

auto inputIsHighFunction(rack::engine::Module const *module, int inputId) -> std::function<bool()> {
  return [module, inputId]() -> bool { return inputIsHigh(module, inputId); };
}

auto inputIsHighOrButtonIsPressedFunction(rack::engine::Module const *module, int inputId, int buttonId)
    -> std::function<bool()> {
  return [module, inputId, buttonId]() -> bool {
    return inputIsHigh(module, inputId) || buttonIsPressed(module, buttonId);
  };
}

auto rotationFunction(rack::engine::Module const *module, int knobId, int cvId, int avId) -> std::function<float()> {
  return [module, knobId, cvId, avId]() -> float { return rotation(module, knobId, cvId, avId); };
}

auto scaledRotationFunction(rack::engine::Module const *module, int knobId, Range const &range)
    -> std::function<float()> {
  return [module, knobId, range]() -> float { return scaledRotation(module, knobId, range); };
}

auto taperedAndScaledRotationFunction(rack::engine::Module const *module, int knobId, taper::FixedTaper const &taper,
                                      Range const &range) -> std::function<float()> {
  return [module, knobId, &taper, &range]() -> float { return taperedAndScaledRotation(module, knobId, taper, range); };
}

auto taperedAndScaledRotationFunction(rack::engine::Module const *module, int knobId, int cvId, int avId,
                                      taper::FixedTaper const &taper, Range const &range) -> std::function<float()> {
  return [module, knobId, cvId, avId, &taper, &range]() -> float {
    return taperedAndScaledRotation(module, knobId, cvId, avId, taper, range);
  };
}

// Instantiate for toggles with 2, 3, and 4 states
template void configToggle<2>(rack::engine::Module const *module, int toggleId, std::string const &toggleName,
                              std::array<std::string, 2> const &stateNames, int initialState);
template void configToggle<3>(rack::engine::Module const *module, int toggleId, std::string const &toggleName,
                              std::array<std::string, 3> const &stateNames, int initialState);
template void configToggle<4>(rack::engine::Module const *module, int toggleId, std::string const &toggleName,
                              std::array<std::string, 4> const &stateNames, int initialState);

} // namespace dhe
