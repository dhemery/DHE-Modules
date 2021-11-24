#pragma once

#include "components/range.h"
#include "signals/shapes.h"

namespace dhe {
namespace blossom {
namespace spin_speed {
static auto constexpr range = Range{-10.F, 10.F};
static auto constexpr knob_taper = -0.8F;

static constexpr auto scale(float normalized) -> float {
  return range.scale(SShape::apply(normalized, knob_taper));
}

static constexpr auto normalize(float scaled) -> float {
  return SShape::invert(range.normalize(scaled), knob_taper);
}

struct KnobMap {
  static auto constexpr default_value = 1.F;
  static auto constexpr unit = " Hz";

  static constexpr auto to_display(float value) -> float {
    return scale(value);
  }

  static constexpr auto to_value(float display) -> float {
    return normalize(display);
  }
};
} // namespace spin_speed

struct SpinSpeed {
  using KnobMap = spin_speed::KnobMap;

  static constexpr auto scale(float normalized) -> float {
    return spin_speed::scale(normalized);
  }

  static constexpr auto normalize(float scaled) -> float {
    return spin_speed::normalize(scaled);
  }
};

} // namespace blossom
} // namespace dhe
