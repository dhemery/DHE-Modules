#pragma once

#include "components/range.h"

#include <string>
#include <vector>

namespace dhe {

struct Voltage {
  static auto constexpr unit = " V";
};

template <typename TVoltage> struct VoltageRange {
  static auto constexpr display_range() -> Range { return TVoltage::range(); };
  static auto constexpr unit = Voltage::unit;

  static inline auto value(float rotation) -> float {
    return display_range().scale(rotation);
  }
};

struct UnipolarVoltage : public VoltageRange<UnipolarVoltage> {
  static auto constexpr range() -> Range { return Range{0.F, 10.F}; }
  static auto constexpr label = "0–10 V";
};

struct BipolarVoltage : public VoltageRange<BipolarVoltage> {
  static auto constexpr range() -> Range { return Range{-5.F, 5.F}; }
  static auto constexpr label = "±5 V";
};

struct Voltages {
  using ValueType = int;
  using ItemType = Range;

  enum { Bipolar, Unipolar };

  static auto constexpr unit = Voltage::unit;

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

  static inline auto value(float rotation, int index) -> float {
    return items()[index].scale(rotation);
  }
};

} // namespace dhe
