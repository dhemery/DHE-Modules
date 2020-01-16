#pragma once

#include "CommonConfig.h"
#include "components/Range.h"
#include "controls/CommonInputs.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>
#include <functional>
#include <string>

namespace dhe {
class Range;

class LevelKnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override {
    auto const rotation = getValue();
    return range()->scale(rotation);
  }

  void setDisplayValue(float displayValue) override {
    auto const normalized = range()->normalize(displayValue);
    setValue(normalized);
  }

  void setRangeSupplier(std::function<Range const *()> const &supplier) { this->range = supplier; }

private:
  std::function<Range const *()> range;
};

static inline void configLevelKnob(rack::engine::Module *module, int knobId,
                                   std::function<Range const *()> const &rangeSupplier, std::string const &name,
                                   float initialPosition) {
  module->configParam<LevelKnobParamQuantity>(knobId, 0.F, 1.F, initialPosition, name, " V");
  auto knobParamQuantity = dynamic_cast<LevelKnobParamQuantity *>(module->paramQuantities[knobId]);
  knobParamQuantity->setRangeSupplier(rangeSupplier);
}

/**
 * Configures the param and display for a level knob with a range selected by a switch.
 */
static inline void configLevelKnob(rack::engine::Module *module, int knobId, int switchId,
                                   std::string const &name = "Level", float initialRotation = centeredRotation) {
  auto const rangeSupplier
      = [module, switchId]() -> Range const * { return selectedRange<2>(module->params[switchId], signalRanges); };
  configLevelKnob(module, knobId, rangeSupplier, name, initialRotation);
}

/**
 * Configures the param and display for a level knob with a fixed range.
 */
static inline void configLevelKnob(rack::engine::Module *module, int knobId, Range const &range,
                                   std::string const &name = "Level", float initialRotation = centeredRotation) {
  auto const rangeSupplier = [range]() -> Range const * { return &range; };
  configLevelKnob(module, knobId, rangeSupplier, name, initialRotation);
}

/**
 * Configures the param and display for a level range switch.
 */
static inline void configLevelRangeSwitch(rack::engine::Module *module, int switchId,
                                          std::string const &name = "Level Range", int initialState = 1) {
  static auto const stateNames = std::array<std::string, 2>{"±5 V", "0–10 V"};
  configToggle<2>(module, switchId, name, stateNames, initialState);
}
} // namespace dhe
