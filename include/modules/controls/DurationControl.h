#pragma once

#include <array>

#include "modules/controls/ModulatedKnob.h"
#include "util/range.h"

namespace DHE {

class DurationControl : public ModulatedKnob {
  static ConstantParam defaultRangeSwitch;

public:
  explicit DurationControl(Param &knob, Param &rangeSwitch = defaultRangeSwitch,
                           Input &cvInput = defaultCvInput,
                           Param &avParam = defaultAvParam)
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
