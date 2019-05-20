#include <array>
#include <iomanip>
#include <sstream>

#include <engine/Module.hpp>

#include "modules/controls/Duration.h"
#include "util/sigmoid.h"

namespace DHE {

auto Duration::range(float switchPosition) -> Range const * {
  auto const selection = static_cast<int>(switchPosition);
  return ranges[selection];
}

auto Duration::range(Param &rangeSwitchParam) -> Range const * {
  return range(rangeSwitchParam.getValue());
}

// Note that each range is of the form [n, 1000n].
static Range constexpr shortRange{0.001f, 1.f};
static Range constexpr mediumRange{0.01f, 10.f};
static Range constexpr longRange{0.1f, 100.f};

/**
 * Each duration range is of the form [n, 1000n]. Given ranges of that form,
 * this curvature tapers the rotation so a knob positioned dead center yields
 * a duration equal to 1/10 of the range's upper bound (to within 7 decimal
 * places).
 */
static float constexpr knobTaperCurvature = 0.8018017;

const std::array<Range const *, 3> Duration::ranges{&shortRange, &mediumRange,
                                                    &longRange};

void Duration::config(rack::engine::Module *module, int knobParamId,
                      int rangeSwitchParamId) {
  module->configParam<Duration::RangeSwitchParamQuantity>(
      rangeSwitchParamId, 0.f, 2.f, 1.f, "Duration Range");
  module->configParam<Duration::KnobParamQuantity>(knobParamId, 0.f, 1.f, 0.5f,
                                                   "Duration", "s");
  auto durationKnob = dynamic_cast<Duration::KnobParamQuantity *>(
      module->paramQuantities[knobParamId]);
  durationKnob->rangeSwitchId = rangeSwitchParamId;
}

ConstantParam Duration::Control::constantMediumDurationRangeSwitch{1.f};

auto Duration::Control::seconds() -> float {
  auto const tapered = Sigmoid::j_shape.taper(rotation(), knobTaperCurvature);
  return Duration::range(rangeSwitch)->scale(tapered);
}

Duration::Control::Control(Param &knob, Param &rangeSwitch, Input &cvInput,
                           Param &avParam)
    : ModulatedKnob{knob, cvInput, avParam}, rangeSwitch{rangeSwitch} {}

auto Duration::RangeSwitchParamQuantity::getDisplayValueString()
    -> std::string {
  auto const range = Duration::range(getValue());
  auto stringStream = std::ostringstream{}
                      << std::fixed << std::setprecision(getDisplayPrecision())
                      << range->lower_bound << "–" << range->upper_bound << "s";
  return stringStream.str();
}

auto Duration::KnobParamQuantity::getDisplayValue() -> float {
  auto const value = getValue();
  auto const tapered = Sigmoid::j_taper(value, knobTaperCurvature);
  return range()->scale(tapered);
}

void Duration::KnobParamQuantity::setDisplayValue(float displayValue) {
  auto const normalized = range()->normalize(displayValue);
  auto const deTapered = Sigmoid::j_taper(normalized, -knobTaperCurvature);
  setValue(deTapered);
}

auto Duration::KnobParamQuantity::range() -> Range const * {
  return Duration::range(module->params[rangeSwitchId]);
}
} // namespace DHE
