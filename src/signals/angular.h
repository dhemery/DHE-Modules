#pragma once

#include "common-inputs.h"
#include "components/cxmath.h"
#include "components/range.h"

namespace dhe {

struct Angle {
  static auto constexpr range = Range{-pi, pi};
  static auto constexpr display_range = Range{-180.F, 180.F};
  static auto constexpr unit = "˚";

  static inline auto radians(float rotation) -> float {
    return range.scale(rotation);
  }
};
} // namespace dhe
