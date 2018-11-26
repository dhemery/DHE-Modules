#include <cmath>

#include "util/range.h"
#include "util/sigmoid.h"

namespace DHE {
namespace Sigmoid {
static constexpr auto sigmoid_range = Range{-1.f, 1.f};
static constexpr auto proportion_range = Range{0.f, 1.f};

auto inverse(float input, float curvature) -> float {
  static constexpr auto precision = 1e-4f;
  static constexpr auto max_curvature = 1.0f - precision;
  static constexpr auto curvature_range = Range{-max_curvature, max_curvature};

  curvature = curvature_range.clamp(curvature);
  input = sigmoid_range.clamp(input);

  return (input - input * curvature) /
         (curvature - std::abs(input) * 2.0f * curvature + 1.0f);
}

auto j_taper(float input, float curvature) -> float {
  return inverse(proportion_range.clamp(input), curvature);
}

auto s_taper(float input, float curvature) -> float {
  const auto scaled = sigmoid_range.scale(input);
  const auto tapered = curve(scaled, curvature);
  return sigmoid_range.normalize(tapered);
}

auto curvature(float input) -> float {
  // This curvature creates a gentle S curve, increasing sensitivity in the
  // middle of the input range and decreasing sensitivity toward the extremes.
  static constexpr auto gentle_s = 0.65f;
  auto scaled = sigmoid_range.scale(input);
  return curve(scaled, gentle_s);
}
} // namespace Sigmoid
} // namespace DHE
