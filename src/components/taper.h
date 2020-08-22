#pragma once

#include <utility>

#include "sigmoid.h"

namespace dhe {

namespace taper {

class Taper {
public:
  constexpr Taper(Range range, float quadrant_factor,
                  float default_curvature = 0.F)
      : range_{range}, quadrant_factor_{quadrant_factor},
        default_curvature_{default_curvature} {}

  constexpr auto apply(float proportion, float curvature) const -> float {
    return normalized(curved(curvature, safe(scaled(proportion))));
  };

  constexpr auto apply(float proportion) const -> float {
    return normalized(curved(default_curvature_, safe(scaled(proportion))));
  };

  constexpr auto invert(float tapered, float curvature) const -> float {
    return apply(tapered, -curvature);
  };

  constexpr auto invert(float tapered) const -> float {
    return apply(tapered, -default_curvature_);
  };

private:
  constexpr auto curved(float curvature, float input) const -> float {
    return sigmoid::curve(input, curvature * quadrant_factor_);
  }

  constexpr auto safe(float input) const -> float {
    return range_.clamp(input);
  }
  constexpr auto scaled(float proportion) const -> float {
    return range_.scale(proportion);
  }
  constexpr auto normalized(float scaled) const -> float {
    return range_.normalize(scaled);
  }

  Range const range_;
  float const quadrant_factor_;
  float const default_curvature_;
};

static auto constexpr j = Taper{Range{0.F, 1.F}, 1};
static auto constexpr s = Taper{sigmoid::range, -1};
static auto constexpr tapers = std::array<taper::Taper const *, 2>{&j, &s};

static constexpr auto s_with_curvature(float curvature) -> Taper {
  return Taper{sigmoid::range, -1, curvature};
}

static constexpr auto j_with_curvature(float curvature) -> Taper {
  return Taper{Range{0.F, 1.F}, 1, curvature};
}
} // namespace taper
} // namespace dhe
