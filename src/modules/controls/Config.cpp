#include "modules/controls/Config.h"

#include "modules/controls/Inputs.h"

namespace dhe {

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
  nonConst(module)->configParam<FrameWidgetParamQuantity<N>>(paramId, 0.F, (float) (N - 1), (float) initialState,
                                                             paramName);
  auto const controlDisplay = dynamic_cast<FrameWidgetParamQuantity<N> *>(module->paramQuantities[paramId]);
  controlDisplay->setFrameNames(stateNames);
}

void configAttenuator(rack::engine::Module *module, int knobId, std::string const &knobName) {
  configPercentageKnob(module, knobId, knobName, attenuatorRange);
}

void configAttenuverter(rack::engine::Module *module, int knobId, std::string const &knobName) {
  configPercentageKnob(module, knobId, knobName, attenuverterRange);
}

void configButton(rack::engine::Module *module, int buttonId, std::string const &buttonName,
                  std::array<std::string, 2> const &stateNames, int initialState) {
  configFrameWidgetStates<2>(module, buttonId, buttonName, stateNames, initialState);
}

void configGain(rack::engine::Module *module, int knobId, std::string const &knobName) {
  configPercentageKnob(module, knobId, knobName, gainRange);
}

void configKnob(rack::engine::Module *module, int knobId, std::string const &knobName, std::string const &units,
                Range const &range, float initialRotation) {
  nonConst(module)->configParam(knobId, 0.F, 1.F, initialRotation, knobName, units, 0.F, range.size(),
                                range.lowerBound);
}

void configPercentageKnob(rack::engine::Module *module, int knobId, std::string const &knobName, Range const &range) {
  nonConst(module)->configParam(knobId, 0.F, 1.F, 0.5F, knobName, "%", 0.F, range.size() * 100.F,
                                range.lowerBound * 100.F);
}

template <int N>
void configToggle(rack::engine::Module *module, int toggleId, std::string const &toggleName,
                  std::array<std::string, N> const &stateNames, int initialState) {
  configFrameWidgetStates<N>(module, toggleId, toggleName, stateNames, initialState);
}

// Instantiate for toggles with 2, 3, and 4 states
template void configToggle<2>(rack::engine::Module *module, int toggleId, std::string const &toggleName,
                              std::array<std::string, 2> const &stateNames, int initialState);
template void configToggle<3>(rack::engine::Module *module, int toggleId, std::string const &toggleName,
                              std::array<std::string, 3> const &stateNames, int initialState);
template void configToggle<4>(rack::engine::Module *module, int toggleId, std::string const &toggleName,
                              std::array<std::string, 4> const &stateNames, int initialState);

} // namespace dhe
