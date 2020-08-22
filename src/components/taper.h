#pragma once

#include "sigmoid.h"

namespace dhe {

namespace taper {
class VariableTaper {
public:
  virtual auto apply(float proportion, float curvature) const -> float = 0;
  auto invert(float tapered, float curvature) const -> float {
    return apply(tapered, -curvature);
  };
};

class VariableJTaper : public VariableTaper {
  static auto constexpr input_range = Range{0.F, 1.F};
  static auto constexpr quadrant_factor = 1;

  static constexpr auto curved(float curvature, float input) -> float {
    return sigmoid::curve(input, curvature * quadrant_factor);
  }

  static constexpr auto safe(float input) -> float {
    return input_range.clamp(input);
  }
  static constexpr auto scaled(float proportion) -> float {
    return input_range.scale(proportion);
  }
  static constexpr auto normalized(float scaled) -> float {
    return input_range.normalize(scaled);
  }

public:
  auto apply(float proportion, float curvature) const -> float override {
    return normalized(curved(curvature, safe(scaled(proportion))));
  }
};

class VariableSTaper : public VariableTaper {
  static auto constexpr input_range = sigmoid::range;
  static auto constexpr quadrant_factor = -1;

  static constexpr auto normalized(float scaled) -> float {
    return input_range.normalize(scaled);
  }
  static constexpr auto curved(float curvature, const float input) -> float {
    return sigmoid::curve(input, curvature * quadrant_factor);
  }
  static constexpr auto safe(const float input) -> float {
    return input_range.clamp(input);
  }
  static constexpr auto scaled(float proportion) -> float {
    return input_range.scale(proportion);
  }

public:
  auto apply(float proportion, float curvature) const -> float override {
    return normalized(curved(curvature, safe(scaled(proportion))));
  }
};

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
  VariableJTaper taper_{};
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
  VariableSTaper taper_{};
  float curvature_;
};

static auto constexpr variable_j_taper = VariableJTaper{};
static auto constexpr variable_s_taper = VariableSTaper{};
static auto constexpr variable_tapers =
    std::array<taper::VariableTaper const *, 2>{&variable_j_taper,
                                                &variable_s_taper};
} // namespace taper
} // namespace dhe
