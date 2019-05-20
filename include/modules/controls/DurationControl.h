#pragma once

#include <array>
#include <iomanip>
#include <sstream>

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

#include "modules/controls/ModulatedKnob.h"
#include "util/range.h"
#include "util/sigmoid.h"

namespace DHE {

class DurationControl : public ModulatedKnob {
  static ConstantParam constantMediumDurationRangeSwitch;

public:
  explicit DurationControl(
      Param &knob, Param &rangeSwitch = constantMediumDurationRangeSwitch,
      Input &cvInput = constant0VoltageInput,
      Param &avParam = constantFullyRotatedKnobParam)
      : ModulatedKnob{knob, cvInput, avParam}, rangeSwitch{rangeSwitch} {}

  auto seconds() -> float;

  static const Range shortRange;
  static const Range mediumRange;
  static const Range longRange;
  static const std::array<Range const *, 3> ranges;

private:
  Param &rangeSwitch;
};

class DurationRangeSwitch : public rack::engine::ParamQuantity {
public:
  std::string getDisplayValueString() override {
    auto const selection = static_cast<int>(getValue());
    auto const range = DurationControl::ranges[selection];
    auto stringStream = std::ostringstream{}
                        << std::fixed << std::setprecision(3 - selection)
                        << range->lower_bound << "–" << range->upper_bound
                        << "s";
    return stringStream.str();
  }
};

class DurationKnob : public rack::engine::ParamQuantity {
  static auto constexpr taperCurvature = 0.8018017;

public:
  float getDisplayValue() override {
    auto const value = getValue();
    auto const tapered = Sigmoid::j_taper(value, taperCurvature);
    return range()->scale(tapered);
  }

  void setDisplayValue(float displayValue) override {
    auto const normalized = range()->normalize(displayValue);
    auto const deTapered = Sigmoid::j_taper(normalized, -taperCurvature);
    setValue(deTapered);
  }

  auto range() -> Range const * {
    auto const switchValue = module->params[rangeSwitchId].getValue();
    auto const selection = static_cast<int>(switchValue);
    return DurationControl::ranges[selection];
  }

  int rangeSwitchId{0};
};
} // namespace DHE
