#pragma once

#include "components/cxmath.h"
#include "components/range.h"

namespace dhe {
template <typename T> struct LinearRange {
  static auto constexpr min = T::min;
  static auto constexpr max = T::max;

  static auto constexpr range() -> Range { return Range{min, max}; }

  static auto constexpr scale(float normalized) -> float {
    return cx::scale(normalized, min, max);
  }

  static auto constexpr normalize(float scaled) -> float {
    return cx::normalize(scaled, min, max);
  }
};

template <typename Bounds, typename Shape> struct TaperedRange {
  static auto constexpr min = Bounds::min;
  static auto constexpr max = Bounds::max;

  static auto constexpr range() -> Range { return Range{min, max}; }

  static auto constexpr scale(float normalized) -> float {
    return cx::scale(Shape::taper(normalized), min, max);
  }

  static auto constexpr normalize(float scaled) -> float {
    return Shape::invert(cx::normalize(scaled, min, max));
  }
};

template <typename T> struct ScaledKnobMap {
  auto to_display(float value) const -> float { return T::scale(value); }
  auto to_value(float display) const -> float { return T::normalize(display); }
};

} // namespace dhe
