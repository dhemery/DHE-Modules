#pragma once

#include "ranged.h"

namespace dhe {

struct RadiansBounds {
  static auto constexpr min = -pi;
  static auto constexpr max = pi;
};

struct DegreesBounds {
  static auto constexpr min = -180.F;
  static auto constexpr max = 180.F;
};

struct AngleKnobMap : ScaledKnobMap<LinearRange<DegreesBounds>> {
  static auto constexpr unit = "˚";
  static auto constexpr default_value = 0.F;
};

struct Angle : LinearRange<RadiansBounds> {
  using KnobMap = AngleKnobMap;

  static inline auto radians(float normalized) -> float {
    return scale(normalized);
  }
};
} // namespace dhe
