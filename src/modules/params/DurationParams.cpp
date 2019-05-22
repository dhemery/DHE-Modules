#include <array>
#include <iomanip>
#include <sstream>

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
    auto const range = getRange();
    auto stringStream =
        std::ostringstream{}
        << std::fixed << std::setprecision(getDisplayPrecision())
        << range->lower_bound << "–" << range->upper_bound << "s";
    return stringStream.str();
  }

  std::function<Range const *()> getRange;
};

class KnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override {
    auto const value = getValue();
    auto const tapered = Sigmoid::j_taper(value, knobTaperCurvature);
    return getRange()->scale(tapered);
  }

  void setDisplayValue(float displayValue) override {
    auto const normalized = getRange()->normalize(displayValue);
    auto const deTapered = Sigmoid::j_taper(normalized, -knobTaperCurvature);
    setValue(deTapered);
  }

  std::function<Range const *()> getRange;
};

void configKnob(rack::engine::Module *module, int knobId,
                std::function<Range const *()> const &getRange) {
  module->configParam<KnobParamQuantity>(knobId, 0.f, 1.f, 0.5f, "Duration",
                                         "s");
  auto durationKnob =
      dynamic_cast<KnobParamQuantity *>(module->paramQuantities[knobId]);
  durationKnob->getRange = getRange;
}

void configSwitch(rack::engine::Module *module, int switchId,
                  std::function<Range const *()> const &getRange) {
  module->configParam<RangeSwitchParamQuantity>(switchId, 0.f, 2.f, 1.f,
                                                "Duration Range");
  auto rangeSwitch = dynamic_cast<RangeSwitchParamQuantity *>(
      module->paramQuantities[switchId]);
  rangeSwitch->getRange = getRange;
}
} // namespace Duration
} // namespace DHE
