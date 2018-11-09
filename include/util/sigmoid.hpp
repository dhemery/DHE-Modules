#pragma once

#include <cmath>

#include "range.hpp"

namespace DHE {
constexpr auto SIGMOID_RANGE = Range{-1.0f, 1.0f};

inline float sigmoid(float x, float curvature) {
  static constexpr auto precision = 1e-4f;
  static constexpr auto max_curvature = 1.0f - precision;
  static const auto curvature_range = Range{-max_curvature, max_curvature};

  curvature = curvature_range.clamp(curvature);
  x = SIGMOID_RANGE.clamp(x);

  return (x - x * curvature) /
         (curvature - std::abs(x) * 2.0f * curvature + 1.0f);
}
} // namespace DHE
