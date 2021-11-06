#pragma once

#include "components/cxmath.h"
#include "components/range.h"

namespace dhe {

struct Angle {
  // value is radians
  static auto constexpr range() -> Range { return Range{-tau, tau}; }
  // display degrees
  static auto constexpr display_multiplier = 180.F / tau;
  static auto constexpr display_offset = 0.F;
  static auto constexpr unit = "˚";
};
} // namespace dhe
