#include <array>

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

#include "modules/controls/Controls.h"
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
                std::function<Range const *()> const &range,
                std::string const &name, float initialPosition) {
  module->configParam<KnobParamQuantity>(knobId, 0.f, 1.f, initialPosition,
                                         name, " V");
  auto knobParamQuantity =
      dynamic_cast<KnobParamQuantity *>(module->paramQuantities[knobId]);
  knobParamQuantity->range = range;
}

void configKnob(rack::engine::Module *module, int knobId, int switchId,
                std::string const &name, float initialPosition) {
  auto getRange = control::range::selection<2>(module, switchId, level::ranges);
  configKnob(module, knobId, getRange, name, initialPosition);
}

void configKnob(rack::engine::Module *module, int knobId, Range const &range,
                std::string const &name, float initialPosition) {
  auto getRange = [range]() -> Range const * { return &range; };
  configKnob(module, knobId, getRange, name, initialPosition);
}

void configSwitch(rack::engine::Module *module, int switchId,
                  std::string const &name, int initialPosition) {
  module->configParam<RangeSwitchParamQuantity>(switchId, 0.f, 1.f,
                                                initialPosition, name);
}
} // namespace level
} // namespace dhe
