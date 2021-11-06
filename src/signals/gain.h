#pragma once

#include "components/range.h"

namespace dhe {

struct Attenuator {
  static auto constexpr range() -> Range { return Range{0.F, 1.F}; }
  static auto constexpr display_multiplier = 100.F;
  static auto constexpr display_offset = 0.F;
  static auto constexpr unit = "%";
};

struct Attenuverter {
  static auto constexpr range() -> Range { return Range{-1.F, 1.F}; }
  static auto constexpr display_multiplier = 100.F;
  static auto constexpr display_offset = 0.F;
  static auto constexpr unit = "%";
};

struct Gain {
  static auto constexpr range() -> Range { return Range{0.F, 2.F}; }
  static auto constexpr display_multiplier = 100.F;
  static auto constexpr display_offset = 0.F;
  static auto constexpr unit = "%";

  static inline auto value(float base, float cv) -> float {
    static auto const gain_per_volt = 0.1F * range().size();
    return base + cv * gain_per_volt;
  }
};

using Percentage = Attenuator;
} // namespace dhe
