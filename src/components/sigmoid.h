#pragma once
#include "cxmath.h"
#include "range.h"

#include <array>

namespace dhe {
namespace sigmoid {

/**
 * Applies a sigmoid function to the input.
 * <p>
 * The curvature determines the shape and intensity of the transfer function.
 * A positive curvature applies an S-shaped transfer function.
 * A curvature of 0 applies a linear transfer function.
 * A negative curvature applies an inverted S-shaped transfer function.
 * @param input the input to the sigmoid function
 * @param curvature the intensity and direction of the curvature
 * @return the sigmoid function result
 */
static constexpr auto curve(float input, float curvature) -> float {
  return (input - input * curvature) /
         (curvature - cx::abs(input) * 2.0F * curvature + 1.0F);
}

/**
 * The range over which the curve() function's input and curvature are defined.
 * Input and curvature values within this range will always produce outputs in
 * this range. The curve() function's behavior for values outside this range is
 * undefined.
 */
static auto constexpr domain = Range{-1.F, 1.F};

static auto constexpr safe_curvature_range = Range{-0.9999F, 0.9999F};

class Taper {
public:
  constexpr Taper(Range input_domain, float quadrant_factor,
                  float default_curvature = 0.F)
      : domain_{input_domain}, quadrant_factor_{quadrant_factor},
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
    return domain_.clamp(input);
  }
  constexpr auto scaled(float proportion) const -> float {
    return domain_.scale(proportion);
  }
  constexpr auto normalized(float scaled) const -> float {
    return domain_.normalize(scaled);
  }

  Range const domain_;
  float const quadrant_factor_;
  float const default_curvature_;
};

static auto constexpr j_taper = Taper{Range{0.F, 1.F}, 1};
static auto constexpr s_taper = Taper{sigmoid::domain, -1};
static auto constexpr tapers = std::array<Taper const *, 2>{&j_taper, &s_taper};

static constexpr auto s_taper_with_curvature(float curvature) -> Taper {
  return Taper{sigmoid::domain, -1, curvature};
}

static constexpr auto j_taper_with_curvature(float curvature) -> Taper {
  return Taper{Range{0.F, 1.F}, 1, curvature};
}
} // namespace sigmoid
} // namespace dhe
