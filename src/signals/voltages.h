#pragma once

#include "components/range.h"

#include <string>
#include <vector>

namespace dhe {

template <typename TVoltage> struct VoltageRange {
  static auto constexpr display_range() -> Range { return TVoltage::range(); };
  static auto constexpr unit = " V";

  static inline auto volts(float rotation) -> float {
    return display_range().scale(rotation);
  }

  static inline auto rotation(float volts) -> float {
    return display_range().normalize(volts);
  }

  struct KnobMapper {
    auto to_display_value(float rotation) const -> float {
      return volts(rotation);
    }

    auto to_rotation(float volts) const -> float { return rotation(volts); }
  };
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
  enum Selection { Bipolar, Unipolar };
  static auto constexpr unit = " V";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{BipolarVoltage::label, UnipolarVoltage::label};
    return labels;
  }

  static inline auto select(int selection) -> Range const & {
    static auto const ranges =
        std::vector<Range>{BipolarVoltage::range(), UnipolarVoltage::range()};
    return ranges[selection];
  }

  static inline auto volts(float rotation, int selection) -> float {
    return select(selection).scale(rotation);
  }

  static inline auto rotation(float volts, int selection) -> float {
    return select(selection).normalize(volts);
  }

  struct KnobMapper {
    auto to_display_value(float rotation) const -> float {
      return volts(rotation, range_);
    }

    auto to_rotation(float volts) const -> float {
      return rotation(volts, range_);
    }

    void select_range(Selection selection) { range_ = selection; }

  private:
    Selection range_{Unipolar};
  };
};

} // namespace dhe
