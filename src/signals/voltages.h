#pragma once

#include "components/range.h"

#include <string>
#include <vector>

namespace dhe {

namespace voltage {
static auto constexpr unit = " V";
} // namespace voltage

template <typename T> struct DisplayableVoltageRange {
  static auto constexpr unit = voltage::unit;
  static inline auto display_range() -> Range { return T::range(); }

  static inline auto volts(float rotation) -> float {
    return T::range().scale(rotation);
  }
};

struct BipolarVoltage : DisplayableVoltageRange<BipolarVoltage> {
  static auto constexpr label = "±5 V";
  static inline auto range() -> Range { return {-5.F, 5.F}; }
};

struct UnipolarVoltage : DisplayableVoltageRange<UnipolarVoltage> {
  static auto constexpr label = "0–10 V";
  static inline auto range() -> Range { return {0.F, 10.F}; }
};

enum class VoltageRangeId { Bipolar, Unipolar };

struct VoltageRanges {
  struct KnobMapper;
  using Selection = VoltageRangeId;
  static auto constexpr unit = voltage::unit;

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{BipolarVoltage::label, UnipolarVoltage::label};
    return labels;
  }

  static inline auto volts(float rotation, VoltageRangeId range_id) -> float {
    return range(range_id).scale(rotation);
  }

  static inline auto rotation(float volts, VoltageRangeId range_id) -> float {
    return range(range_id).normalize(volts);
  }

private:
  static inline auto range(VoltageRangeId range_id) -> Range const & {
    static auto const ranges =
        std::vector<Range>{BipolarVoltage::range(), UnipolarVoltage::range()};
    return ranges[(int)range_id];
  }
};

struct VoltageRanges::KnobMapper {
  auto to_display_value(float rotation) const -> float {
    return volts(rotation, range_id_);
  }

  auto to_rotation(float volts) const -> float {
    return rotation(volts, range_id_);
  }

  void select_range(VoltageRangeId id) { range_id_ = id; }

private:
  VoltageRangeId range_id_{};
};

} // namespace dhe
