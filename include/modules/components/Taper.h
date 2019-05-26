#pragma once

#include "util/Sigmoid.h"

namespace dhe {

namespace taper {
  auto constexpr sigmoidRange = Range{-1.F, 1.F};

  class VariableTaper {
  public:
    virtual auto apply(float proportion, float curvature) const -> float = 0;
    virtual auto invert(float taperedValue, float curvature) const -> float { return apply(taperedValue, -curvature); };
  };

  class VariableJTaper : public VariableTaper {
  public:
    auto apply(float proportion, float curvature) const -> float override {
      return sigmoid::curve(proportion, curvature);
    }
  };

  class VariableSTaper : public VariableTaper {
  public:
    auto apply(float proportion, float curvature) const -> float override {
      auto const sigmoidInput = sigmoidRange.scale(proportion);
      auto const sigmoidOutput = sigmoid::curve(sigmoidInput, -curvature);
      return sigmoidRange.normalize(sigmoidOutput);
    }
  };

  class FixedTaper {
  public:
    virtual auto apply(float normalValue) const -> float = 0;
    virtual auto invert(float taperedValue) const -> float = 0;
  };

  class FixedJTaper : public FixedTaper {
  public:
    explicit constexpr FixedJTaper(float curvature) : curvature{curvature} {}

    auto apply(float proportion) const -> float override { return taper.apply(proportion, curvature); }

    auto invert(float sTaperedValue) const -> float override { return taper.invert(sTaperedValue, curvature); }

  private:
    VariableJTaper taper{};
    float curvature;
  };

  class FixedSTaper : public FixedTaper {
  public:
    explicit constexpr FixedSTaper(float curvature) : curvature{curvature} {}

    auto apply(float proportion) const -> float override { return taper.apply(proportion, curvature); }

    auto invert(float sTaperedValue) const -> float override { return taper.invert(sTaperedValue, curvature); }

  private:
    VariableSTaper taper{};
    float curvature;
  };

  static auto constexpr variableJTaper = VariableJTaper{};
  static auto constexpr variableSTaper = VariableSTaper{};
} // namespace taper
} // namespace dhe
