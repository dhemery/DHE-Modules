#pragma once

#include "components/range.h"
#include "signals/shape-signals.h"

namespace dhe {
namespace blossom {
namespace spin_speed {
static auto constexpr range = Range{-10.F, 10.F};
} // namespace spin_speed

struct SpinSpeed {
  static constexpr auto scale(float normalized, float modulation = 0.F)
      -> float {
    return range.scale(apply_taper(normalized) + modulation);
  }

  static constexpr auto normalize(float scaled) -> float {
    return invert_taper(range.normalize(scaled));
  }

private:
  static auto constexpr &range = spin_speed::range;
  static auto constexpr taper_curvature = -0.8F;

  static constexpr auto apply_taper(float normalized) -> float {
    return SShape::apply(normalized, taper_curvature);
  }

  static constexpr auto invert_taper(float tapered) -> float {
    return SShape::invert(tapered, taper_curvature);
  }
};

} // namespace blossom
} // namespace dhe
