#pragma once

#include "sigmoid.h"

namespace dhe {

namespace taper {
class VariableTaper {
public:
  constexpr VariableTaper(Range range, float quadrant_factor)
      : range_{range}, quadrant_factor_{quadrant_factor} {}

  constexpr auto apply(float proportion, float curvature) const -> float {
    return normalized(curved(curvature, safe(scaled(proportion))));
  };

  constexpr auto invert(float tapered, float curvature) const -> float {
    return apply(tapered, -curvature);
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
};

static auto constexpr variable_j_taper = VariableTaper{Range{0.F, 1.F}, 1};
static auto constexpr variable_s_taper = VariableTaper{sigmoid::range, -1};
static auto constexpr variable_tapers =
    std::array<taper::VariableTaper const *, 2>{&variable_j_taper,
                                                &variable_s_taper};

class FixedTaper {
public:
  virtual auto apply(float normal_value) const -> float = 0;
  virtual auto invert(float tapered_value) const -> float = 0;
};

class FixedJTaper : public FixedTaper {
public:
  explicit constexpr FixedJTaper(float curvature) : curvature_{curvature} {}

  auto apply(float proportion) const -> float override {
    return taper_.apply(proportion, curvature_);
  }

  auto invert(float tapered) const -> float override {
    return taper_.invert(tapered, curvature_);
  }

private:
  VariableTaper taper_{variable_j_taper};
  float curvature_;
};

class FixedSTaper : public FixedTaper {
public:
  explicit constexpr FixedSTaper(float curvature) : curvature_{curvature} {}

  auto apply(float proportion) const -> float override {
    return taper_.apply(proportion, curvature_);
  }

  auto invert(float tapered) const -> float override {
    return taper_.invert(tapered, curvature_);
  }

private:
  VariableTaper taper_{variable_s_taper};
  float curvature_;
};
} // namespace taper
} // namespace dhe
