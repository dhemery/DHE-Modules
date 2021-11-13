#pragma once

#include "ranged.h"

namespace dhe {

struct Angle : RangedFloat<Angle> {
  static auto constexpr min = -pi;
  static auto constexpr max = pi;
  static auto constexpr display_min = -180.F;
  static auto constexpr display_max = 180.F;
  static auto constexpr display_default = 0.F;
  static auto constexpr unit = "˚";

  static inline auto radians(float normalized) -> float {
    return scale(normalized);
  }
};
} // namespace dhe
