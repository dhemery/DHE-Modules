#pragma once

#include "components/range.h"
#include "components/sigmoid.h"

#include "rack.hpp"

namespace dhe {
namespace blossom {

struct SpinSpeed {
  struct KnobMap;

  static inline auto scale(float normalized) -> float {
    return range().scale(SShape::apply(normalized, -0.8F));
  }

  static inline auto normalize(float scaled) -> float {
    return SShape::invert(range().normalize(scaled), -0.8F);
  }

  static inline auto range() -> Range {
    static auto const range = Range{-10.F, 10.F};
    return range;
  }
};

struct SpinSpeed::KnobMap {
  static auto constexpr default_value = 1.F;
  static auto constexpr unit = " Hz";

  static inline auto to_display(float value) -> float { return scale(value); }

  static inline auto to_value(float display) -> float {
    return normalize(display);
  }
};

} // namespace blossom
} // namespace dhe
