#pragma once

#include <array>

#include "ModulatedKnob.h"

#include "util/range.h"

namespace DHE {

class Level : public ModulatedKnob {
  static ConstantParam defaultRangeSwitch;

public:
  explicit Level(rack::engine::Param &knob,
                 rack::engine::Param &rangeSwitch = defaultRangeSwitch,
                 rack::engine::Input &cvInput = defaultCvInput);

  auto voltage() -> float;

  static const Range unipolar_range;
  static const Range bipolar_range;
  static const std::array<Range const *, 2> ranges;

private:
  rack::engine::Param &rangeSwitch;
};

} // namespace DHE
