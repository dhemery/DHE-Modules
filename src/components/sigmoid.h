#pragma once

#include "cxmath.h"

namespace dhe {
namespace sigmoid {
/**
 * Applies a sigmoid function to an input.
 * <p>
 * The curvature determines the shape and intensity of the transfer function.
 * A positive curvature applies an S-shaped transfer function.
 * A curvature of 0 applies a linear transfer function.
 * A negative curvature applies an inverted S-shaped transfer function.
 * <p>
 * A valid input is in the closed interval [-1.F, 1.F].
 * A valid curvature is in the open interval [-1.F, 1.F].
 *
 * @param curvature the intensity and direction of the curvature
 * @param input the input to the sigmoid function
 * @return the sigmoid function result
 */
static constexpr auto curve(float input, float curvature) -> float {
  return (input - input * curvature) /
         (curvature - cx::abs(input) * 2.0F * curvature + 1.0F);
}
} // namespace sigmoid
} // namespace dhe
