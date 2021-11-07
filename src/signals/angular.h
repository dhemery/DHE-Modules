#pragma once

#include "components/cxmath.h"
#include "components/range.h"

namespace dhe {

struct Angle {
  static auto constexpr display_range = Range{-180.F, 180.F};
  static auto constexpr unit = "˚";

  static inline auto radians(float rotation) -> float {
    return rotation * tau - pi;
  }
};
} // namespace dhe
