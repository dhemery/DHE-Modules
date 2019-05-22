#include <array>

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

#include "modules/controls/Duration.h"
#include "modules/params/DurationParams.h"
#include "util/sigmoid.h"

namespace DHE {

namespace Duration {

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
    auto const value = getValue();
    auto const tapered = Sigmoid::j_taper(value, knobTaperCurvature);
    return range()->scale(tapered);
  }

  void setDisplayValue(float displayValue) override {
    auto const normalized = range()->normalize(displayValue);
    auto const deTapered = Sigmoid::j_taper(normalized, -knobTaperCurvature);
    setValue(deTapered);
  }

  std::function<Range const *()> range;
};

void configKnob(rack::engine::Module *module, int knobId,
                std::function<Range const *()> const &getRange) {
  module->configParam<KnobParamQuantity>(knobId, 0.f, 1.f, 0.5f, "Duration",
                                         " s");
  auto knobParamQuantity =
      dynamic_cast<KnobParamQuantity *>(module->paramQuantities[knobId]);
  knobParamQuantity->range = getRange;
}

void configKnob(rack::engine::Module *module, int knobId, Range const &range) {
  auto const getRange = [range]() -> Range const * { return &range; };
  configKnob(module, knobId, getRange);
}

void configKnob(rack::engine::Module *module, int knobId, int switchId) {
  auto *switchParam = &module->params[switchId];
  auto const getRange = [switchParam]() -> Range const * {
    auto const selection = static_cast<int>(switchParam->getValue());
    return Duration::ranges[selection];
  };
  configKnob(module, knobId, getRange);
}

void configSwitch(rack::engine::Module *module, int switchId) {
  module->configParam<RangeSwitchParamQuantity>(switchId, 0.f, 2.f, 1.f,
                                                "Duration Range");
}
} // namespace Duration
} // namespace DHE
