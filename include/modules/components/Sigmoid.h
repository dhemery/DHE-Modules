#pragma once
#include "Range.h"

#include <array>
#include <cmath>

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
  static inline auto curve(float input, float curvature) -> float {
    return (input - input * curvature) / (curvature - std::abs(input) * 2.0F * curvature + 1.0F);
  }

  /**
   * The range over which the curve() function's input and curvature are defined. Input and curvature values within this
   * range will always produce outputs in this range. The curve() function's behavior for values outside this range is
   * undefined.
   */
  static auto constexpr range = Range{-1.F, 1.F};

  static auto constexpr safeCurvatureRange = Range{-0.9999F, 0.9999F};
} // namespace sigmoid
} // namespace dhe
