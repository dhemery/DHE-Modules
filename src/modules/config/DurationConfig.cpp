#include <array>
#include <iomanip>
#include <sstream>

#include <engine/Module.hpp>

#include "modules/components/Duration.h"
#include "modules/config/DurationConfig.h"
#include "util/sigmoid.h"

namespace DHE {

void Duration::config(rack::engine::Module *module, int knobParamId,
                      std::function<Range const *()> const &getRange) {
  module->configParam<Duration::KnobParamQuantity>(knobParamId, 0.f, 1.f, 0.5f,
                                                   "Duration", "s");
  auto durationKnob = dynamic_cast<Duration::KnobParamQuantity *>(
      module->paramQuantities[knobParamId]);
  durationKnob->getRange = getRange;
}

void Duration::config(rack::engine::Module *module, int knobParamId,
                      int rangeSwitchParamId,
                      std::function<Range const *()> const &getRange) {
  module->configParam<Duration::RangeParamQuantity>(rangeSwitchParamId, 0.f,
                                                    2.f, 1.f, "Duration Range");
  auto rangeSwitch = dynamic_cast<Duration::RangeParamQuantity *>(
      module->paramQuantities[rangeSwitchParamId]);
  rangeSwitch->getRange = getRange;
  config(module, knobParamId, getRange);
}

auto Duration::RangeParamQuantity::getDisplayValueString() -> std::string {
  auto const range = getRange();
  auto stringStream = std::ostringstream{}
                      << std::fixed << std::setprecision(getDisplayPrecision())
                      << range->lower_bound << "–" << range->upper_bound << "s";
  return stringStream.str();
}

auto Duration::KnobParamQuantity::getDisplayValue() -> float {
  auto const value = getValue();
  auto const tapered = Sigmoid::j_taper(value, knobTaperCurvature);
  return getRange()->scale(tapered);
}

void Duration::KnobParamQuantity::setDisplayValue(float displayValue) {
  auto const normalized = getRange()->normalize(displayValue);
  auto const deTapered = Sigmoid::j_taper(normalized, -knobTaperCurvature);
  setValue(deTapered);
}
} // namespace DHE
