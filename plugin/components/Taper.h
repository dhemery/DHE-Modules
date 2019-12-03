#pragma once

#include "Sigmoid.h"

namespace dhe {

namespace taper {
  class VariableTaper {
  public:
    virtual auto apply(float proportion, float curvature) const -> float = 0;
    virtual auto invert(float taperedValue, float curvature) const -> float { return apply(taperedValue, -curvature); };
  };

  class VariableJTaper : public VariableTaper {
  public:
    auto apply(float proportion, float curvature) const -> float override {
      static auto constexpr safeProportionRange = Range{0.F, 1.F};
      auto const safeProportion = safeProportionRange.clamp(proportion);
      return sigmoid::curve(safeProportion, curvature);
    }
  };

  class VariableSTaper : public VariableTaper {
  public:
    auto apply(float proportion, float curvature) const -> float override {
      auto const input = sigmoid::range.scale(proportion);
      auto const safeInput = sigmoid::range.clamp(input);
      auto const output = sigmoid::curve(safeInput, -curvature);
      return sigmoid::range.normalize(output);
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
  static auto constexpr variableTapers = std::array<taper::VariableTaper const *, 2>{&variableJTaper, &variableSTaper};
} // namespace taper
} // namespace dhe
