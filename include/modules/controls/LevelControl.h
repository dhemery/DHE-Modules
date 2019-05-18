#pragma once

#include <array>

#include "ModulatedKnob.h"

#include "util/range.h"

namespace DHE {

class LevelControl : public ModulatedKnob {
  static ConstantParam defaultRangeSwitch;

public:
  explicit LevelControl(Param &knob, Param &rangeSwitch = defaultRangeSwitch,
                        Input &cvInput = defaultCvInput,
                        Param &avParam = defaultAvParam)
      : ModulatedKnob{knob, cvInput, avParam}, rangeSwitch{rangeSwitch} {};

  auto voltage() -> float;
  auto range() -> Range const *;

  static const Range unipolar_range;
  static const Range bipolar_range;
  static const std::array<Range const *, 2> ranges;

private:
  rack::engine::Param &rangeSwitch;
};

} // namespace DHE
