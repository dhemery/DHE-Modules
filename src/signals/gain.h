#pragma once

#include "ranged.h"

namespace dhe {

struct Attenuator : RangedFloat<Attenuator> {
  static auto constexpr min = 0.F;
  static auto constexpr max = 1.F;
  static auto constexpr unit = "%";
  static auto constexpr display_min = 0.F;
  static auto constexpr display_max = 100.F;
  static auto constexpr display_default = 100.F;
};

struct Attenuverter : RangedFloat<Attenuverter> {
  static auto constexpr min = -1.F;
  static auto constexpr max = 1.F;
  static auto constexpr unit = "%";
  static auto constexpr display_min = -100.F;
  static auto constexpr display_max = 100.F;
  static auto constexpr display_default = 0.F;
};

struct Gain : RangedFloat<Gain> {
  static auto constexpr min = 0.F;
  static auto constexpr max = 2.F;
  static auto constexpr unit = "%";
  static auto constexpr display_min = 0.F;
  static auto constexpr display_max = 200.F;
  static auto constexpr display_default = 100.F;
};

using Percentage = Attenuator;
} // namespace dhe
