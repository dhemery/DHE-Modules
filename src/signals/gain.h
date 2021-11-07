#pragma once

#include "components/range.h"

namespace dhe {

struct Attenuator {
  static auto constexpr range() -> Range { return Range{0.F, 1.F}; }
  static auto constexpr display_range() -> Range { return Range{0.F, 100.F}; }
  static auto constexpr unit = "%";
};

struct Attenuverter {
  static auto constexpr range() -> Range { return Range{-1.F, 1.F}; }
  static auto constexpr display_range() -> Range {
    return Range{-100.F, 100.F};
  }
  static auto constexpr unit = "%";

  static inline auto value(float rotation) -> float {
    return range().scale(rotation);
  }
};

struct Gain {
  static auto constexpr range() -> Range { return Range{0.F, 2.F}; }
  static auto constexpr display_range() -> Range { return Range{0.F, 200.F}; }
  static auto constexpr unit = "%";

  static inline auto value(float rotation) -> float {
    return range().scale(rotation);
  }
};

using Percentage = Attenuator;
} // namespace dhe
