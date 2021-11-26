#pragma once

#include "components/range.h"
#include "ranged.h"

#include <array>
#include <string>
#include <vector>

namespace dhe {
enum class VoltageRangeId { Bipolar, Unipolar };

namespace internal {
namespace voltage {
static auto constexpr unit = " V";

namespace bipolar {
static auto constexpr range = Range{-5.F, 5.F};
struct Quantity {
  static auto constexpr default_value = 0.F;
  static auto constexpr &display_range = bipolar::range;
  static auto constexpr &range = bipolar::range;
  static auto constexpr unit = voltage::unit;
};
} // namespace bipolar

namespace unipolar {
static auto constexpr range = Range{0.F, 10.F};
struct Quantity {
  static auto constexpr default_value = 5.F;
  static auto constexpr &display_range = unipolar::range;
  static auto constexpr &range = unipolar::range;
  static auto constexpr unit = voltage::unit;
};
} // namespace unipolar

static auto constexpr ranges =
    std::array<Range, 2>{bipolar::range, unipolar::range};
static auto constexpr labels =
    std::array<char const *, ranges.size()>{"±5 V", "0–10 V"};

static inline auto range(VoltageRangeId id) -> Range {
  return ranges[static_cast<size_t>(id)];
}
} // namespace voltage
} // namespace internal

struct VoltageRanges {
  using ValueType = VoltageRangeId;
  static auto constexpr size = internal::voltage::ranges.size();
  static inline auto labels() -> std::vector<std::string> {
    return {internal::voltage::labels.cbegin(),
            internal::voltage::labels.cend()};
  }
};

struct BipolarVoltage : LinearKnob<internal::voltage::bipolar::Quantity> {};
struct UnipolarVoltage : LinearKnob<internal::voltage::unipolar::Quantity> {};

struct Voltage {
  static auto constexpr unit = internal::voltage::unit;

  static inline auto scale(float normalized, VoltageRangeId range_id) -> float {
    return internal::voltage::range(range_id).scale(normalized);
  }
  static inline auto normalize(float scaled, VoltageRangeId range_id) -> float {
    return internal::voltage::range(range_id).normalize(scaled);
  }

  struct KnobMap {
    static auto constexpr unit = Voltage::unit;
    static auto constexpr default_value =
        BipolarVoltage::KnobMap::default_value;

    auto to_display(float value) const -> float {
      return scale(value, range_id_);
    }

    auto to_value(float display) const -> float {
      return normalize(display, range_id_);
    }

    void select_range(VoltageRangeId id) { range_id_ = id; }

  private:
    VoltageRangeId range_id_{VoltageRangeId::Bipolar};
  };
};
} // namespace dhe
