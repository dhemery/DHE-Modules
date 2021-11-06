#pragma once

#include "components/cxmath.h"
#include "components/range.h"

namespace dhe {

struct Large {
  static auto constexpr svg_file = "knob-large";
};

struct Medium {
  static auto constexpr svg_file = "knob-medium";
};

struct Small {
  static auto constexpr svg_file = "knob-small";
};

struct Tiny {
  static auto constexpr svg_file = "knob-tiny";
};

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

struct Angle {
  // value is radians
  static auto constexpr range() -> Range { return Range{-tau, tau}; }
  // display degrees
  static auto constexpr display_multiplier = 180.F / tau;
  static auto constexpr display_offset = 0.F;
  static auto constexpr unit = "˚";
};

using Percentage = Attenuator;

} // namespace dhe
