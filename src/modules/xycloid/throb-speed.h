#pragma once

#include "components/range.h"
#include "signals/shape-signals.h"

namespace dhe {
namespace xycloid {
namespace throb_speed {
static auto constexpr range = Range{-10.F, 10.F};
} // namespace throb_speed

struct ThrobSpeed {
  static constexpr auto scale(float normalized, float modulation = 0.F)
      -> float {
    return range.scale(apply_taper(normalized) + modulation);
  }

  static constexpr auto normalize(float scaled) -> float {
    return invert_taper(range.normalize(scaled));
  }

private:
  static auto constexpr curvature = -0.8F;
  static auto constexpr &range = throb_speed::range;

  static constexpr auto apply_taper(float normalized) -> float {
    return SShape::apply(normalized, curvature);
  }

  static constexpr auto invert_taper(float tapered) -> float {
    return SShape::invert(tapered, curvature);
  }
};

} // namespace xycloid
} // namespace dhe
