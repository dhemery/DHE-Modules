#pragma once

#include "cxmath.h"
#include "range.h"

#include <array>
#include <ostream>

namespace dhe {
struct Sigmoid {
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
};

struct JShape {
  static auto constexpr apply(float input, float curvature) -> float {
    return Sigmoid::curve(input, curvature);
  }

  static auto constexpr invert(float input, float curvature) -> float {
    return Sigmoid::curve(input, -curvature);
  }
};

struct SShape {
  static auto constexpr apply(float input, float curvature) -> float {
    return scale_down(Sigmoid::curve(scale_up(input), -curvature));
  }

  static auto constexpr invert(float input, float curvature) -> float {
    return apply(input, -curvature);
  }

private:
  static auto constexpr scale_up(float input) -> float {
    return cx::scale(input, -1.F, 1.F);
  }

  static auto constexpr scale_down(float curved) -> float {
    return cx::normalize(curved, -1.F, 1.F);
  }
};

struct Shape {
  enum class Id { J, S };
  static constexpr auto j = Id::J;
  static constexpr auto s = Id::S;

  static auto constexpr apply(float input, Id id, float curvature) -> float {
    return id == s ? SShape::apply(input, curvature)
                   : JShape::apply(input, curvature);
  }
};
} // namespace dhe
