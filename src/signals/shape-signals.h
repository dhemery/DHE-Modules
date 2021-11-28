#pragma once

#include "components/range.h"
#include "components/sigmoid.h"

#include <array>

namespace dhe {

namespace curvature {
static auto constexpr range = Range{-0.9999F, 0.9999F};
} // namespace curvature

namespace shape {
static auto constexpr labels = std::array<char const *, 2>{"J", "S"};
} // namespace shape

struct JShape {
  static constexpr auto apply(float input, float curvature) -> float {
    return sigmoid::curve(input, curvature);
  }

  static constexpr auto invert(float input, float curvature) -> float {
    return sigmoid::curve(input, -curvature);
  }
};

struct SShape {
  static constexpr auto apply(float input, float curvature) -> float {
    return scale_down(sigmoid::curve(scale_up(input), -curvature));
  }

  static constexpr auto invert(float input, float curvature) -> float {
    return apply(input, -curvature);
  }

private:
  static constexpr auto scale_up(float input) -> float {
    return cx::scale(input, -1.F, 1.F);
  }

  static constexpr auto scale_down(float curved) -> float {
    return cx::normalize(curved, -1.F, 1.F);
  }
};

struct Shape {
  enum class Id { J, S };

  static constexpr auto apply(float input, Id id, float curvature) -> float {
    return id == Id::S ? SShape::apply(input, curvature)
                       : JShape::apply(input, curvature);
  }
};

struct Curvature {
  static auto constexpr &range = curvature::range;
  /**
   * This curvature gives a curvature knob a gentle inverted S taper, increasing
   * sensitivity in the middle of the knob normalize and decreasing sensitivity
   * toward the extremes.
   */
  static auto constexpr taper_curvature = 0.65F;

  static constexpr auto scale(float normalized) -> float {
    return range.scale(SShape::apply(normalized, taper_curvature));
  }

  static constexpr auto normalize(float scaled) -> float {
    return SShape::invert(range.normalize(scaled), taper_curvature);
  }
};

} // namespace dhe
