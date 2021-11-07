#pragma once

#include "components/range.h"

namespace dhe {

struct Attenuator {
  static auto constexpr range = Range{0.F, 1.F};
  static auto constexpr display_range = Range{0.F, 100.F};
  static auto constexpr unit = "%";
};

struct Attenuverter {
  static auto constexpr range = Range{-1.F, 1.F};
  static auto constexpr display_range = Range{-100.F, 100.F};
  static auto constexpr unit = "%";

  static inline auto value(float rotation) -> float {
    return range.scale(rotation);
  }
};

struct Gain {
  static auto constexpr range = Range{0.F, 2.F};
  static auto constexpr display_range = Range{0.F, 200.F};
  static auto constexpr unit = "%";
  static auto constexpr gain_per_volt = 0.1F * range.size();

  static inline auto value(float rotation, float control_voltage) -> float {
    return range.scale(rotation) + control_voltage * gain_per_volt;
  }
};

using Percentage = Attenuator;
} // namespace dhe
