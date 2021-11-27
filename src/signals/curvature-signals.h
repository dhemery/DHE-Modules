#pragma once

#include "components/range.h"
#include "shapes.h"

namespace dhe {

namespace curvature {
static auto constexpr range = Range{-0.9999F, 0.9999F};
} // namespace curvature

struct Curvature {
  /**
   * This curvature gives a curvature knob a gentle inverted S taper, increasing
   * sensitivity in the middle of the knob normalize and decreasing sensitivity
   * toward the extremes.
   */
  static auto constexpr taper_curvature = 0.65F;
  static auto constexpr &range = curvature::range;

  static auto constexpr scale(float normalized) -> float {
    return range.scale(SShape::apply(normalized, taper_curvature));
  }

  static auto constexpr normalize(float scaled) -> float {
    return SShape::invert(range.normalize(scaled), taper_curvature);
  }
};
} // namespace dhe
