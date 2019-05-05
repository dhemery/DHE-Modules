#pragma once

#include <array>

#include "modules/controls/Common.h"
#include "util/range.h"

namespace DHE {

class Duration {
  static ConstantParam defaultRangeSwitch;

public:
  explicit Duration(rack::engine::Param &knob,
                    rack::engine::Param &rangeSwitch = defaultRangeSwitch,
                    rack::engine::Input &cvInput = defaultCvInput)
      : knob{knob}, rangeSwitch{rangeSwitch}, cvInput{cvInput} {}

  auto seconds() -> float;

  static const Range short_range;
  static const Range medium_range;
  static const Range long_range;
  static const std::array<Range const *, 3> ranges;

private:
  rack::engine::Param &knob;
  rack::engine::Param &rangeSwitch;
  rack::engine::Input &cvInput;
};

} // namespace DHE
