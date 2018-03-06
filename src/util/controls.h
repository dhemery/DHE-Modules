#pragma once

#include "interval.h"
#include "sigmoid.h"

namespace DHE {

namespace Duration {
constexpr auto RANGE_MAX_TO_MIN_RATIO = 1000.f;
constexpr auto MEDIUM_RANGE_MAX = 10.0f;
constexpr auto MEDIUM_RANGE_MIN = MEDIUM_RANGE_MAX/RANGE_MAX_TO_MIN_RATIO;

constexpr auto SCALE_STEP = 10.f;
constexpr auto SHORT_RANGE = Interval{MEDIUM_RANGE_MIN/SCALE_STEP, MEDIUM_RANGE_MAX/SCALE_STEP};
constexpr auto MEDIUM_RANGE = Interval{MEDIUM_RANGE_MIN, MEDIUM_RANGE_MAX};
constexpr auto LONG_RANGE = Interval{MEDIUM_RANGE_MIN*SCALE_STEP, MEDIUM_RANGE_MAX*SCALE_STEP};

inline const Interval range(float switch_value) {
  return switch_value < 0.5f ? SHORT_RANGE :
         switch_value < 1.5f ? MEDIUM_RANGE : LONG_RANGE;
}

inline float scaled(float rotation, const Interval &range) {
  static constexpr auto KNOB_CURVATURE = 0.8f; // Yields ~1/10 of max at center position
  auto shaped_rotation = sigmoid(rotation, KNOB_CURVATURE);
  return range.scale(shaped_rotation);
}

}

namespace Level {

inline const Interval &range(float switch_value) {
  return switch_value > 0.5f ? UNIPOLAR_CV : BIPOLAR_CV;
}

inline float scaled(float rotation, const Interval &range) {
  return range.scale(rotation);
}
}
}