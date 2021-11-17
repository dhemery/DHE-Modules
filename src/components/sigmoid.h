#pragma once
#include "./cxmath.h"
#include "./range.h"

#include <array>
#include <ostream>

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

static constexpr auto invert(float input, float curvature) -> float {
  return curve(input, -curvature);
}

/**
 * The range over which the curve() function's input and curvature are defined.
 * InPort and curvature values within this select will always produce outputs_
 * in this range. The curve() function's behavior for values outside this select
 * is undefined.
 */
static auto constexpr domain = Range{-1.F, 1.F};

enum class ShapeId { J, S };

struct SShape {
  static auto constexpr apply(float input, float curvature) -> float {
    return scale_down(curve(scale_up(input), -curvature));
  }

  static auto constexpr invert(float input, float curvature) -> float {
    return apply(input, curvature);
  }

private:
  static auto constexpr scale_up(float input) -> float {
    return cx::scale(input, -1.F, 1.F);
  }

  static auto constexpr scale_down(float curved) -> float {
    return cx::normalize(curved, -1.F, 1.F);
  }
};

struct JShape {
  static auto constexpr apply(float input, float curvature) -> float {
    return curve(input, curvature);
  }

  static auto constexpr invert(float input, float curvature) -> float {
    return apply(input, -curvature);
  }
};

struct Shape {
  static auto constexpr apply(ShapeId id, float input, float curvature)
      -> float {
    return id == ShapeId::S ? SShape::apply(input, curvature)
                            : JShape::apply(input, curvature);
  }

  static auto constexpr invert(ShapeId id, float input, float curvature)
      -> float {
    return id == ShapeId::S ? SShape::invert(input, curvature)
                            : JShape::invert(input, curvature);
  }
};
} // namespace sigmoid
} // namespace dhe
