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

  template <typename TParam, typename TPort>
  static inline auto value(TParam knob, TPort cv_input) -> float {
    static auto constexpr gain_per_volt = 0.1F * range.size();
    auto const rotation = knob.getValue();
    auto const cv = cv_input.getVoltage();
    return range.scale(rotation) + cv * gain_per_volt;
  }
};

using Percentage = Attenuator;
} // namespace dhe
