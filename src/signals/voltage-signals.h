#pragma once

#include "components/range.h"
#include "linear-signals.h"

#include <array>

namespace dhe {
enum class VoltageRangeId { Bipolar, Unipolar };

namespace voltage {
static auto constexpr unit = " V";
static auto constexpr bipolar_range = Range{-5.F, 5.F};
static auto constexpr unipolar_range = Range{0.F, 10.F};
static auto constexpr ranges =
    std::array<Range, 2>{bipolar_range, unipolar_range};
static auto constexpr labels =
    std::array<char const *, ranges.size()>{"±5 V", "0–10 V"};

} // namespace voltage

struct BipolarVoltage : LinearSignal<BipolarVoltage> {
  static auto constexpr &range = voltage::bipolar_range;
};
struct UnipolarVoltage : LinearSignal<UnipolarVoltage> {
  static auto constexpr &range = voltage::unipolar_range;
};

struct Voltage {
  static inline auto range(VoltageRangeId id) -> Range {
    return voltage::ranges[static_cast<size_t>(id)];
  }

  static inline auto scale(float normalized, VoltageRangeId range_id) -> float {
    return range(range_id).scale(normalized);
  }

  static inline auto normalize(float scaled, VoltageRangeId range_id) -> float {
    return range(range_id).normalize(scaled);
  }
};
} // namespace dhe
