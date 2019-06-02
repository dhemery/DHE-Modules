#include "modules/controls/LevelConfig.h"

#include "modules/controls/Functions.h"
#include "modules/controls/Inputs.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

namespace dhe {

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

void configLevelKnob(rack::engine::Module const *module, int knobId,
                     std::function<Range const *()> const &rangeSupplier, std::string const &name,
                     float initialPosition) {
  nonConst(module)->configParam<LevelKnobParamQuantity>(knobId, 0.F, 1.F, initialPosition, name, " V");
  auto knobParamQuantity = dynamic_cast<LevelKnobParamQuantity *>(module->paramQuantities[knobId]);
  knobParamQuantity->setRangeSupplier(rangeSupplier);
}

void configLevelKnob(rack::engine::Module const *module, int knobId, int switchId, std::string const &name,
                     float initialRotation) {
  auto getRange = rangeSelectorFunction<2>(module, switchId, signalRanges);
  configLevelKnob(module, knobId, getRange, name, initialRotation);
}

void configLevelKnob(rack::engine::Module const *module, int knobId, Range const &range, std::string const &name,
                     float initialRotation) {
  auto getRange = [range]() -> Range const * { return &range; };
  configLevelKnob(module, knobId, getRange, name, initialRotation);
}

void configLevelRangeSwitch(rack::engine::Module const *module, int switchId, std::string const &name,
                            int initialState) {
  static auto const stateNames = std::array<std::string, 2>{"±5 V", "0–10 V"};
  configToggle<2>(module, switchId, name, stateNames, initialState);
}
} // namespace dhe
