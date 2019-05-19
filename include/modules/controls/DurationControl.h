#pragma once

#include <array>

#include "modules/controls/ModulatedKnob.h"
#include "util/range.h"

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

  static const Range short_range;
  static const Range medium_range;
  static const Range long_range;
  static const std::array<Range const *, 3> ranges;

private:
  Param &rangeSwitch;
};

} // namespace DHE
