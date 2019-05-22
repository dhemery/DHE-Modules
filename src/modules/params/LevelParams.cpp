#include <array>

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

#include "modules/controls/Level.h"
#include "modules/params/LevelParams.h"

namespace dhe {

namespace level {

class RangeSwitchParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValueString() -> std::string override {
    return getValue() < 0.5 ? "±5 V" : "0–10 V";
  }
};

class KnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override {
    auto const rotation = getValue();
    return range()->scale(rotation);
  }

  void setDisplayValue(float displayValue) override {
    auto const normalized = range()->normalize(displayValue);
    setValue(normalized);
  }

  std::function<Range const *()> range;
};

void configKnob(rack::engine::Module *module, int knobId,
                std::function<Range const *()> const &range) {
  module->configParam<KnobParamQuantity>(knobId, 0.f, 1.f, 0.5f, "Level", " V");
  auto knobParamQuantity =
      dynamic_cast<KnobParamQuantity *>(module->paramQuantities[knobId]);
  knobParamQuantity->range = range;
}

void configKnob(rack::engine::Module *module, int knobId, int switchId) {
  auto switchParam = &module->params[switchId];
  auto getRange = [switchParam]() -> Range const * {
    return level::range(static_cast<int>(switchParam->getValue()));
  };
  configKnob(module, knobId, getRange);
}

void configKnob(rack::engine::Module *module, int knobId, Range const &range) {
  auto getRange = [range]() -> Range const * { return &range; };
  configKnob(module, knobId, getRange);
}

void configSwitch(rack::engine::Module *module, int switchId,
                  int initialPosition) {
  module->configParam<RangeSwitchParamQuantity>(switchId, 0.f, 1.f,
                                                initialPosition, "Level Range");
}
} // namespace level
} // namespace dhe
