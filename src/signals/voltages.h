#pragma once

#include "components/range.h"
#include "enums.h"
#include "ranged.h"

#include <string>
#include <vector>

namespace dhe {

static auto constexpr voltage_unit = " V";

struct BipolarVoltage : RangedFloat<BipolarVoltage> {
  static auto constexpr unit = voltage_unit;
  static auto constexpr min = -5.F;
  static auto constexpr max = 5.F;
  static auto constexpr display_min = min;
  static auto constexpr display_max = max;
  static auto constexpr display_default = 0.F;
  static auto constexpr label = "±5 V";
};

struct UnipolarVoltage : RangedFloat<UnipolarVoltage> {
  static auto constexpr unit = voltage_unit;
  static auto constexpr min = 0.F;
  static auto constexpr max = 10.F;
  static auto constexpr display_min = min;
  static auto constexpr display_max = max;
  static auto constexpr display_default = 5.F;
  static auto constexpr label = "0–10 V";
};

enum class VoltageRangeId { Bipolar, Unipolar };

struct VoltageRanges : Enums<VoltageRangeId, 2> {
  struct KnobMapper;
  static auto constexpr default_rotation = 0.5F;
  static auto constexpr unit = voltage_unit;

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{BipolarVoltage::label, UnipolarVoltage::label};
    return labels;
  }

  static inline auto range(VoltageRangeId id) -> Range const & {
    static auto const ranges =
        std::vector<Range>{BipolarVoltage::range(), UnipolarVoltage::range()};
    return ranges[static_cast<int>(id)];
  }

  static inline auto scale(float normalized, VoltageRangeId range_id) -> float {
    return range(range_id).scale(normalized);
  }

  static inline auto normalize(float scaled, VoltageRangeId range_id) -> float {
    return range(range_id).normalize(scaled);
  }
};

struct VoltageRanges::KnobMapper {
  auto scale(float normalized) const -> float {
    return VoltageRanges::scale(normalized, range_id_);
  }

  auto normalize(float scaled) const -> float {
    return VoltageRanges::normalize(scaled, range_id_);
  }

  void select_range(VoltageRangeId id) { range_id_ = id; }

private:
  VoltageRangeId range_id_{};
};

} // namespace dhe
