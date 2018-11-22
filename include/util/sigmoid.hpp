#pragma once

#include <cmath>

#include "range.hpp"

namespace DHE {
auto constexpr sigmoid_range{Range{-1.0f, 1.0f}};

inline float sigmoid(float x, float curvature) {
  static auto constexpr precision{1e-4f};
  static auto constexpr max_curvature{1.0f - precision};
  static auto constexpr curvature_range{Range{-max_curvature, max_curvature}};

  curvature = curvature_range.clamp(curvature);
  x = sigmoid_range.clamp(x);

  return (x - x * curvature) /
         (curvature - std::abs(x) * 2.0f * curvature + 1.0f);
}
} // namespace DHE
