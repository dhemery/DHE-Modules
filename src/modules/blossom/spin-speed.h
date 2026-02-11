#pragma once

#include "components/range.h"
#include "signals/shape.h"

namespace dhe {
namespace blossom {
namespace spin_speed {
static auto range = Range{-10.F, 10.F};
} // namespace spin_speed

struct SpinSpeed {
  static auto scale(float normalized, float modulation = 0.F) -> float {
    return range.scale(apply_taper(normalized) + modulation);
  }

  static auto normalize(float scaled) -> float {
    return invert_taper(range.normalize(scaled));
  }

private:
  static auto constexpr &range = spin_speed::range;
  static auto constexpr taper_curvature = -0.8F;

  static auto apply_taper(float normalized) -> float {
    return s_shape.apply(normalized, taper_curvature);
  }

  static auto invert_taper(float tapered) -> float {
    return s_shape.invert(tapered, taper_curvature);
  }
};

} // namespace blossom
} // namespace dhe
