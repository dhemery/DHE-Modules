#pragma once

#include "components/range.h"
#include "signals/linear.h"

#include <array>

namespace dhe {
enum class VoltageRangeId { Bipolar, Unipolar };

namespace voltage {
static auto constexpr unit = " V";
static auto bipolar_range = Range{-5.F, 5.F};
static auto unipolar_range = Range{0.F, 10.F};
static auto ranges = std::array<Range, 2>{bipolar_range, unipolar_range};
static auto constexpr labels = std::array<char const *, ranges.size()>{"±5 V", "0–10 V"};

} // namespace voltage

struct BipolarVoltage : LinearSignal<BipolarVoltage> {
  static auto constexpr &range = voltage::bipolar_range;
};
struct UnipolarVoltage : LinearSignal<UnipolarVoltage> {
  static auto constexpr &range = voltage::unipolar_range;
};

struct Voltage {
  static auto range(VoltageRangeId id) -> Range;

  static auto scale(float normalized, VoltageRangeId range_id) -> float;

  static auto normalize(float scaled, VoltageRangeId range_id) -> float;
};
} // namespace dhe
