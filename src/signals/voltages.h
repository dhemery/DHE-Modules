#pragma once

#include "components/range.h"

#include <array>
#include <string>
#include <vector>

namespace dhe {
namespace voltage {
static auto constexpr unit = " V";
static auto constexpr bipolar_range = Range{-5.F, 5.F};
static auto constexpr unipolar_range = Range{0.F, 10.F};

struct Bipolar {
  static auto constexpr &range = bipolar_range;
  static auto constexpr unit = voltage::unit;
};

struct Unipolar {
  static auto constexpr &range = unipolar_range;
  static auto constexpr unit = voltage::unit;
};

template <typename V> struct Mapped : V {
  static auto constexpr &range = V::range;
  static constexpr auto scale(float normalized) -> float {
    return range.scale(normalized);
  }

  static constexpr auto normalize(float scaled) -> float {
    return range.normalize(scaled);
  }

  struct KnobMap {
    static auto constexpr unit = voltage::unit;
    static auto constexpr default_value = scale(0.5F);
    auto to_display(float rotation) const -> float { return scale(rotation); }

    auto to_value(float display) const -> float { return normalize(display); }
  };
};
} // namespace voltage

enum class VoltageRangeId { Bipolar, Unipolar };

namespace voltage_ranges {
static auto constexpr size = 2;
static auto constexpr labels = std::array<char const *, size>{"±5 V", "0–10 V"};
static auto constexpr ranges =
    std::array<Range, size>{voltage::bipolar_range, voltage::unipolar_range};

static inline auto range(VoltageRangeId id) -> Range {
  return ranges[static_cast<size_t>(id)];
}
} // namespace voltage_ranges

struct VoltageRanges {
  using value_type = VoltageRangeId;
  static auto constexpr size = voltage_ranges::size;
  static inline auto labels() -> std::vector<std::string> {
    return {voltage_ranges::labels.cbegin(), voltage_ranges::labels.cend()};
  }
};

struct BipolarVoltage : voltage::Mapped<voltage::Bipolar> {};
struct UnipolarVoltage : voltage::Mapped<voltage::Unipolar> {};

struct Voltage {
  static inline auto scale(float normalized, VoltageRangeId range_id) -> float {
    return voltage_ranges::range(range_id).scale(normalized);
  }
  static inline auto normalize(float scaled, VoltageRangeId range_id) -> float {
    return voltage_ranges::range(range_id).normalize(scaled);
  }

  struct KnobMap {
    static auto constexpr unit = voltage::unit;
    static auto constexpr default_value = 5.F;

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
