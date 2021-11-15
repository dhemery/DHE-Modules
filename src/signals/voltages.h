#pragma once

#include "components/range.h"
#include "enums.h"
#include "ranged.h"

#include <string>
#include <vector>

namespace dhe {

static auto constexpr voltage_unit = " V";

template <typename Bounds>
struct VoltageKnobMap : ScaledKnobMap<LinearRange<Bounds>> {
  static auto constexpr unit = voltage_unit;
  static auto constexpr default_rotation = 0.F;
};

struct BipolarBounds {
  static auto constexpr min = -5.F;
  static auto constexpr max = 5.F;
};

struct BipolarVoltage : LinearRange<BipolarBounds> {
  using KnobMap = VoltageKnobMap<BipolarBounds>;
  static auto constexpr label = "±5 V";
};

struct UnipolarBounds {
  static auto constexpr min = 0.F;
  static auto constexpr max = 10.F;
};

struct UnipolarVoltage : LinearRange<UnipolarBounds> {
  using KnobMap = VoltageKnobMap<UnipolarBounds>;
  static auto constexpr unit = voltage_unit;
  static auto constexpr label = "0–10 V";
};

enum class VoltageRangeId { Bipolar, Unipolar };

struct VoltageRanges : Enums<VoltageRangeId, 2> {
  struct KnobMap;

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{BipolarVoltage::label, UnipolarVoltage::label};
    return labels;
  }

  static inline auto scale(float normalized, VoltageRangeId range_id) -> float {
    return range(range_id).scale(normalized);
  }
  static inline auto normalize(float scaled, VoltageRangeId range_id) -> float {
    return range(range_id).normalize(scaled);
  }

  static inline auto range(VoltageRangeId id) -> Range {
    return ranges()[static_cast<int>(id)];
  }

  static inline auto ranges() -> std::vector<Range> const & {
    static auto const ranges =
        std::vector<Range>{BipolarVoltage::range(), UnipolarVoltage::range()};
    return ranges;
  }
};

struct VoltageRanges::KnobMap {
  static auto constexpr default_rotation = 0.5F;
  static auto constexpr unit = voltage_unit;

  auto to_display(float value) const -> float {
    return scale(value, range_id_);
  }

  auto to_value(float display) const -> float {
    return normalize(display, range_id_);
  }

  void select_range(VoltageRangeId id) { range_id_ = id; }

private:
  VoltageRangeId range_id_{};
};

} // namespace dhe
