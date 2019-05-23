#include <array>

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

#include "modules/controls/Duration.h"
#include "modules/params/DurationParams.h"
#include "util/sigmoid.h"

namespace dhe {

namespace duration {

class RangeSwitchParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValueString() -> std::string override {
    static auto const descriptions =
        std::array<std::string, 3>{"0.001–1.0 s", "0.01–10.0 s", "0.1–100.0 s"};
    auto const selection = static_cast<int>(getValue());
    return descriptions[selection];
  }
};

class KnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override {
    static auto const durationTaperFor = duration::rotationToTaper();
    auto const rotation = getValue();
    return range()->scale(durationTaperFor(rotation));
  }

  void setDisplayValue(float durationSeconds) override {
    static auto const rotationFor = duration::taperToRotation();
    auto const durationTaper = range()->normalize(durationSeconds);
    setValue(rotationFor(durationTaper));
  }

  std::function<Range const *()> range;
};

void configKnob(rack::engine::Module *module, int knobId,
                std::function<Range const *()> const &getRange,
                std::string const &name, float initialPosition) {
  module->configParam<KnobParamQuantity>(knobId, 0.f, 1.f, initialPosition,
                                         name, " s");
  auto knobParamQuantity =
      dynamic_cast<KnobParamQuantity *>(module->paramQuantities[knobId]);
  knobParamQuantity->range = getRange;
}

void configKnob(rack::engine::Module *module, int knobId, Range const &range,
                std::string const &name, float initialPosition) {
  auto const getRange = [range]() -> Range const * { return &range; };
  configKnob(module, knobId, getRange, name, initialPosition);
}

void configKnob(rack::engine::Module *module, int knobId, int switchId,
                std::string const &name, float initialPosition) {
  auto *switchParam = &module->params[switchId];
  auto const getRange = [switchParam]() -> Range const * {
    auto const selection = static_cast<int>(switchParam->getValue());
    return duration::ranges[selection];
  };
  configKnob(module, knobId, getRange, name, initialPosition);
}

void configSwitch(rack::engine::Module *module, int switchId,
                  std::string const &name, int initialPosition) {
  module->configParam<RangeSwitchParamQuantity>(switchId, 0.f, 2.f,
                                                initialPosition, name);
}
} // namespace duration
} // namespace dhe
