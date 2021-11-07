#pragma once

#include "components/range.h"

#include <string>
#include <vector>

namespace dhe {

struct UnipolarVoltage {
  static auto constexpr range() -> Range { return Range{0.F, 10.F}; }
  static auto constexpr display_range() -> Range { return range(); }
  static auto constexpr label = "0–10 V";
  static auto constexpr unit = " V";

  static inline auto value(float rotation) -> float {
    return range().scale(rotation);
  }
};

struct BipolarVoltage {
  static auto constexpr range() -> Range { return Range{-5.F, 5.F}; }
  static auto constexpr display_range() -> Range { return range(); }
  static auto constexpr label = "±5 V";
  static auto constexpr unit = " V";

  static inline auto value(float rotation) -> float {
    return range().scale(rotation);
  }
};

struct VoltageRanges {
  using ItemType = Range;

  enum { Bipolar, Unipolar };

  static auto constexpr unit = " V";

  static inline auto items() -> std::vector<Range> const & {
    static auto const items =
        std::vector<Range>{BipolarVoltage::range(), UnipolarVoltage::range()};
    return items;
  }

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{BipolarVoltage::label, UnipolarVoltage::label};
    return labels;
  }

  static inline auto value(float rotation, int range_selection) -> float {
    return items()[range_selection].scale(rotation);
  }
};

} // namespace dhe
