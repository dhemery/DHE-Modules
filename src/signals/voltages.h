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

  static inline auto scale(float normalized) -> float {
    return T::range().scale(normalized);
  }

  static inline auto normalize(float scaled) -> float {
    return T::range().normalize(scaled);
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

  static inline auto range(VoltageRangeId range_id) -> Range const & {
    static auto const ranges =
        std::vector<Range>{BipolarVoltage::range(), UnipolarVoltage::range()};
    return ranges[(int)range_id];
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
