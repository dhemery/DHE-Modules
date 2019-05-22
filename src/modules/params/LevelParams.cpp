#include <array>
#include <iomanip>
#include <sstream>

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

#include "modules/controls/Level.h"
#include "modules/params/LevelParams.h"

namespace DHE {

namespace Level {

class RangeSwitchParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValueString() -> std::string override {
    auto const range = getRange();
    auto stringStream =
        std::ostringstream{}
        << std::fixed << std::setprecision(getDisplayPrecision())
        << range->lower_bound << "–" << range->upper_bound << "V";
    return stringStream.str();
  }

  std::function<Range const *()> getRange;
};

class KnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override {
    auto const rotation = getValue();
    return getRange()->scale(rotation);
  }

  void setDisplayValue(float displayValue) override {
    auto const normalized = getRange()->normalize(displayValue);
    setValue(normalized);
  }

  std::function<Range const *()> getRange;
};

void configKnob(rack::engine::Module *module, int knobId,
                std::function<Range const *()> const &getRange) {
  module->configParam<KnobParamQuantity>(knobId, 0.f, 1.f, 0.5f, "Level", "V");
  auto knobParamQuantity =
      dynamic_cast<KnobParamQuantity *>(module->paramQuantities[knobId]);
  knobParamQuantity->getRange = getRange;
}

void configSwitch(rack::engine::Module *module, int switchId,
                  int initialPosition,
                  std::function<Range const *()> const &getRange) {
  module->configParam<RangeSwitchParamQuantity>(switchId, 0.f, 1.f,
                                                initialPosition, "Level Range");
  auto switchParamQuantity = dynamic_cast<RangeSwitchParamQuantity *>(
      module->paramQuantities[switchId]);
  switchParamQuantity->getRange = getRange;
}
} // namespace Level
} // namespace DHE
