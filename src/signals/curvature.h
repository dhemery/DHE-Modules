#pragma once

#include "shapes.h"

namespace dhe {

template <typename Shape, typename Curvature> struct TaperedShape {
  static auto constexpr apply(float linear) -> float {
    return Shape::apply(linear, Curvature::curvature);
  }

  static auto constexpr invert(float tapered) -> float {
    return Shape::invert(tapered, Curvature::curvature);
  }
};

template <typename Taper, typename Bounds> struct ScaledShape {
  static auto constexpr scale(float normalized) -> float {
    return cx::scale(Taper::apply(normalized), Bounds::min, Bounds::max);
  }

  static auto constexpr normalize(float scaled) -> float {
    return Taper::invert(cx::normalize(scaled, Bounds::min, Bounds::max));
  }
};

struct CurvatureKnobCurvature {
  /**
   * This curvature gives a curvature knob a gentle inverted S taper, increasing
   * sensitivity in the middle of the knob normalize and decreasing sensitivity
   * toward the extremes.
   */
  static auto constexpr curvature = 0.65F;
};

struct CurvatureBounds {
  static auto constexpr max = 0.9999F;
  static auto constexpr min = -max;
};

struct CurvatureTaper : TaperedShape<SShape, CurvatureKnobCurvature> {};

struct Curvature : ScaledShape<CurvatureTaper, CurvatureBounds> {
  struct KnobMap;
};
;

struct Curvature::KnobMap {
  static auto constexpr unit = "";
  static auto constexpr default_value = 0.F;

  static constexpr auto to_display(float value) -> float {
    return scale(value);
  }

  static constexpr auto to_value(float display) -> float {
    return normalize(display);
  }
};
} // namespace dhe
