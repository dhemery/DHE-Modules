#pragma once
#include "Range.h"

#include <array>
#include <cmath>

namespace dhe {
namespace sigmoid {

  // TODO: Remove comment about clamping (unless I restore clamping)
  /**
   * Applies a sigmoid function to the input.
   * <p>
   * The curvature determines the shape and intensity of the transfer function.
   * A positive curvature applies an S-shaped transfer function.
   * A curvature of 0 applies a linear transfer function.
   * A negative curvature applies an inverted S-shaped transfer function.
   * <p>
   * Before the function is applied:
   * <ul>
   * <li>The input is clamped to the range [-1.0, 1.0].</li>
   * <li>The curvature is clamped to the range [-0.9999, 0.9999].</li>
   * </ul>
   * @param input the input to the sigmoid function
   * @param curvature the intensity and direction of the curvature
   * @return the sigmoid function result
   */
  static inline auto curve(float input, float curvature) -> float {
    return (input - input * curvature) / (curvature - std::abs(input) * 2.0F * curvature + 1.0F);
  }
} // namespace sigmoid
} // namespace dhe
