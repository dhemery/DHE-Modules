#pragma once

namespace dhe {
template <typename Quantity> struct LinearKnob {
  static auto constexpr &range = Quantity::range;

  static auto constexpr scale(float normalized) -> float {
    return range.scale(normalized);
  }

  static auto constexpr normalize(float scaled) -> float {
    return range.normalize(scaled);
  }

  static auto constexpr clamp(float unsafe) -> float {
    return range.clamp(unsafe);
  }

  struct KnobMap {
    static auto constexpr default_value = Quantity::default_value;
    static auto constexpr &display_range = Quantity::display_range;
    static auto constexpr unit = Quantity::unit;

    auto to_display(float value) const -> float {
      return display_range.scale(value);
    }
    auto to_value(float display) const -> float {
      return display_range.normalize(display);
    }
  };
};

} // namespace dhe
