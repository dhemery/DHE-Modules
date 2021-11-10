#pragma once

#include "components/range.h"

#include <string>
#include <vector>

namespace dhe {

struct Voltage {
  static auto constexpr unit = " V";
};

// Maps a knob value (rotation) to and from a display value (volts) for a
// given voltage range.
template <typename TVoltageRange> struct VoltageRangeMapper {
  auto to_value(float volts) const -> float {
    return TVoltageRange::display_range().normalize(volts);
  }

  auto to_display_value(float value) const -> float {
    return TVoltageRange::value(value);
  }
};

template <typename TVoltage> struct VoltageRange {
  static auto constexpr display_range() -> Range { return TVoltage::range(); };
  static auto constexpr unit = Voltage::unit;

  static inline auto value(float rotation) -> float {
    return display_range().scale(rotation);
  }

  using KnobMapper = VoltageRangeMapper<TVoltage>;
};

struct UnipolarVoltage : public VoltageRange<UnipolarVoltage> {
  static auto constexpr range() -> Range { return Range{0.F, 10.F}; }
  static auto constexpr label = "0–10 V";
};

struct BipolarVoltage : public VoltageRange<BipolarVoltage> {
  static auto constexpr range() -> Range { return Range{-5.F, 5.F}; }
  static auto constexpr label = "±5 V";
};

// Maps a knob param value (rotation) to and from a display value (volts) for
// the selected voltage range.
template <typename TVoltages> struct SelectableVoltageRangeMapper {
  auto to_value(float volts) const -> float {
    return TVoltages::select(range_index_).normalize(volts);
  }

  auto to_display_value(float rotation) const -> float {
    return TVoltages::value(rotation, range_index_);
  }

  void select_range(int range_index) { range_index_ = range_index; }

private:
  int range_index_{TVoltages::Unipolar};
};

struct Voltages {
  enum Index { Bipolar, Unipolar };
  using KnobMapper = SelectableVoltageRangeMapper<Voltages>;
  using ValueType = Index;

  static auto constexpr unit = Voltage::unit;

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

  static inline auto value(float rotation, int selection) -> float {
    return select(selection).scale(rotation);
  }
};

} // namespace dhe
