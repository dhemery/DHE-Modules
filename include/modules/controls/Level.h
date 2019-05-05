#pragma once

#include <array>

#include "Common.h"

#include "util/range.h"

namespace DHE {

class Level {
  static ConstantParam defaultRangeSwitch;

public:
  explicit Level(rack::engine::Param &knob,
                 rack::engine::Param &rangeSwitch = defaultRangeSwitch,
                 rack::engine::Input &cvInput = defaultCvInput)
      : knob{knob}, rangeSwitch{rangeSwitch}, cvInput{cvInput} {}

  auto voltage() -> float;

  static const Range unipolar_range;
  static const Range bipolar_range;
  static const std::array<Range const *, 2> ranges;

private:
  rack::engine::Param &knob;
  rack::engine::Param &rangeSwitch;
  rack::engine::Input &cvInput;
};

} // namespace DHE
