#pragma once

#include "range.hpp"
#include "sigmoid.hpp"

namespace DHE {
namespace Taper {
inline float curvature(float rotation) {
  // Scale the rotation to use the entire range of the sigmoid curve.
  auto bipolar_rotation = SIGMOID_RANGE.scale(rotation);

  // This knob curvature gives an S taper that gently increases sensitivity in
  // the middle of the rotation and decreases sensitivity toward the ends.
  constexpr auto KNOB_CURVATURE = -0.65f;

  // Apply the S taper to the bipolar rotation.
  return sigmoid(bipolar_rotation, KNOB_CURVATURE);
}

inline float j(float phase, float rotation) {
  return sigmoid(phase, curvature(rotation));
}

inline float s(float phase, float rotation) {
  // Scale the phase to use the entire range of the sigmoid curve.
  auto bipolar_phase = SIGMOID_RANGE.scale(phase);

  // Invert the curvature so that rotation greater than 0.5 gives an S taper.
  auto s_tapered_bipolar_phase = sigmoid(bipolar_phase, -curvature(rotation));

  // Scale the tapered phase back to the range [0,1].
  return SIGMOID_RANGE.normalize(s_tapered_bipolar_phase);
}

} // namespace Taper
} // namespace DHE
