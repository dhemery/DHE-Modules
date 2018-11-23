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

inline float curvature(float proportion) {
  // Scale the rotation to use the entire range of the sigmoid curve.
  auto bipolar_rotation{sigmoid_range.scale(proportion)};

  // This knob curvature gives an S taper that gently increases sensitivity in
  // the middle of the rotation and decreases sensitivity toward the ends.
  auto constexpr knob_curvature{-0.65f};

  // Apply the S taper to the bipolar rotation.
  return sigmoid(bipolar_rotation, knob_curvature);
}

inline float j_taper(float proportion, float curvature) {
  return sigmoid(proportion, curvature);
}

inline float s_taper(float proportion, float curvature) {
  // Scale the proportion to use the entire range of the sigmoid curve [-1,1].
  auto const scaled{sigmoid_range.scale(proportion)};

  // Invert the curvature so that a proportion greater than 0.5 gives an S
  // shape.
  auto const tapered{sigmoid(scaled, -curvature)};

  // Scale the tapered proportion back to the range [0,1].
  return sigmoid_range.normalize(tapered);
}

} // namespace DHE
