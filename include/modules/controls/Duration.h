#pragma once

#include <array>

#include "modules/controls/ModulatedKnob.h"
#include "util/range.h"

namespace DHE {

class Duration : public ModulatedKnob {
  static ConstantParam defaultRangeSwitch;

public:
  explicit Duration(Param &knob, Param &rangeSwitch = defaultRangeSwitch,
                    Input &cvInput = defaultCvInput)
      : ModulatedKnob{knob, cvInput}, rangeSwitch{rangeSwitch} {}

  auto seconds() -> float;

  static const Range short_range;
  static const Range medium_range;
  static const Range long_range;
  static const std::array<Range const *, 3> ranges;

private:
  Param &rangeSwitch;
};

} // namespace DHE
