#pragma once

#include "signals/ranged.h"

namespace dhe {
namespace cubic {
namespace coefficient {

struct Bounds {
  static auto constexpr min = -2.F;
  static auto constexpr max = 2.F;
};

struct KnobMap : ScaledKnobMap<LinearRange<Bounds>> {
  static auto constexpr default_value = 0.F;
  static auto constexpr unit = "";
};
} // namespace coefficient

struct Coefficient : LinearRange<coefficient::Bounds> {
  using KnobMap = coefficient::KnobMap;
};

} // namespace cubic
} // namespace dhe
