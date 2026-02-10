#pragma once

#include "components/range.h"

#include <array>

namespace dhe {

namespace curvature {
static auto range = Range{-0.9999F, 0.9999F};
} // namespace curvature

namespace shape {
static auto constexpr labels = std::array<char const *, 2>{"J", "S"};
} // namespace shape

struct JShape {
  static auto apply(float input, float curvature) -> float;
  static auto invert(float input, float curvature) -> float;
};

struct SShape {
  static auto apply(float input, float curvature) -> float;
  static auto invert(float input, float curvature) -> float;

private:
  static auto scale_up(float input) -> float;
  static auto scale_down(float curved) -> float;
};

struct Shape {
  enum class Id { J, S };

  static auto apply(float input, Id id, float curvature) -> float;
};

struct Curvature {
  static auto constexpr &range = curvature::range;
  /**
   * This curvature gives a curvature knob a gentle inverted S taper, increasing
   * sensitivity in the middle of the knob normalize and decreasing sensitivity
   * toward the extremes.
   */
  static auto constexpr taper_curvature = 0.65F;

  static auto scale(float normalized) -> float;

  static auto normalize(float scaled) -> float;
};

} // namespace dhe
