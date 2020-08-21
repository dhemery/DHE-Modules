#pragma once

#include "sigmoid.h"

namespace dhe {

namespace taper {
class VariableTaper {
public:
  virtual auto apply(float proportion, float curvature) const -> float = 0;
  virtual auto invert(float tapered_value, float curvature) const -> float {
    return apply(tapered_value, -curvature);
  };
};

class VariableJTaper : public VariableTaper {
public:
  auto apply(float proportion, float curvature) const -> float override {
    static auto constexpr safe_proportion_range = Range{0.F, 1.F};
    auto const safe_proportion = safe_proportion_range.clamp(proportion);
    return sigmoid::curve(safe_proportion, curvature);
  }
};

class VariableSTaper : public VariableTaper {
public:
  auto apply(float proportion, float curvature) const -> float override {
    auto const input = sigmoid::range.scale(proportion);
    auto const safe_input = sigmoid::range.clamp(input);
    auto const output = sigmoid::curve(safe_input, -curvature);
    return sigmoid::range.normalize(output);
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

  auto invert(float s_tapered_value) const -> float override {
    return taper_.invert(s_tapered_value, curvature_);
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

  auto invert(float s_tapered_value) const -> float override {
    return taper_.invert(s_tapered_value, curvature_);
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
