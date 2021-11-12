#pragma once

#include "components/range.h"

#include <string>
#include <vector>

namespace dhe {

namespace voltage {
static auto constexpr unit = " V";

template <typename TVoltage> struct Internal {
  static auto constexpr unit = voltage::unit;
  static inline auto display_range() -> Range { return TVoltage::range(); }
  static inline auto volts(float rotation) -> float {
    return TVoltage::range().scale(rotation);
  }
};
} // namespace voltage

struct BipolarVoltage : voltage::Internal<BipolarVoltage> {
  static auto constexpr label = "±5 V";
  static inline auto range() -> Range { return {-5.F, 5.F}; }
};

struct UnipolarVoltage : voltage::Internal<UnipolarVoltage> {
  static auto constexpr label = "0–10 V";
  static inline auto range() -> Range { return {0.F, 10.F}; }
};

enum class VoltageRange { Bipolar, Unipolar };

struct VoltageRanges {
  struct KnobMapper;
  using Selection = VoltageRange;
  static auto constexpr unit = voltage::unit;

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{BipolarVoltage::label, UnipolarVoltage::label};
    return labels;
  }

  static inline auto volts(float rotation, VoltageRange range) -> float {
    return display_range(range).scale(rotation);
  }

  static inline auto rotation(float volts, VoltageRange range) -> float {
    return display_range(range).normalize(volts);
  }

private:
  static inline auto display_range(VoltageRange range) -> Range const & {
    static auto const ranges =
        std::vector<Range>{BipolarVoltage::range(), UnipolarVoltage::range()};
    return ranges[(int)range];
  }
};

struct VoltageRanges::KnobMapper {
  auto to_display_value(float rotation) const -> float {
    return volts(rotation, range_);
  }

  auto to_rotation(float volts) const -> float {
    return rotation(volts, range_);
  }

  void select_range(VoltageRange range) { range_ = range; }

private:
  VoltageRange range_{VoltageRange::Unipolar};
};

} // namespace dhe
