#pragma once

#include <array>

#include "ModulatedKnob.h"

#include "util/range.h"

namespace dhe {

class LevelControl : public ModulatedKnob {
  static ConstantParam constantUnipolarLevelRangeSwitch;

public:
  explicit LevelControl(Param &knob,
                        Param &rangeSwitch = constantUnipolarLevelRangeSwitch,
                        Input &cvInput = constant0VoltageInput,
                        Param &avParam = constantFullyRotatedKnobParam)
      : ModulatedKnob{knob, cvInput, avParam}, rangeSwitch{rangeSwitch} {};

  auto voltage() -> float;
  auto range() -> Range const *;

  static const Range unipolar_range;
  static const Range bipolar_range;
  static const std::array<Range const *, 2> ranges;

private:
  rack::engine::Param &rangeSwitch;
};

} // namespace dhe
