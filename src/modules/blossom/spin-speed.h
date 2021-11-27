#pragma once

#include "components/range.h"
#include "signals/shape-signals.h"

namespace dhe {
namespace blossom {
namespace spin_speed {
static auto constexpr range = Range{-10.F, 10.F};
} // namespace spin_speed

struct SpinSpeed {
  /**
   * @param rotation the rotation of the throb speed knob
   * @param modulation extra rotation to add after tapering
   */
  static constexpr auto scale(float rotation, float modulation) -> float {
    return range.scale(apply_taper(rotation) + modulation);
  }

  struct KnobMap {
    static auto constexpr default_value = 1.F;
    static auto constexpr unit = " Hz";

    static constexpr auto to_display(float value) -> float {
      return scale(value, 0.F);
    }

    static constexpr auto to_value(float display) -> float {
      return invert_taper(range.normalize(display));
    }
  };

private:
  static auto constexpr curvature = -0.8F;
  static auto constexpr &range = spin_speed::range;

  static constexpr auto apply_taper(float normalized) -> float {
    return SShape::apply(normalized, curvature);
  }

  static constexpr auto invert_taper(float tapered) -> float {
    return SShape::invert(tapered, curvature);
  }
};

} // namespace blossom
} // namespace dhe
