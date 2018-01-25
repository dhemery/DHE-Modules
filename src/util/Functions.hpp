#pragma once

#include <cmath>
#include <functional>

namespace DHE {
inline float clamp(float in, float low, float high) {
  if (in < low)
    return low;
  if (in > high)
    return high;
  return in;
}

inline float curve(float phase, float shape) {
  return shape >= 0.0f ? pow(phase, shape + 1.0f) : 1.0f - curve(1.0f - phase, -shape);
}

inline float scaleToRange(float normalized, float min, float max) {
  return normalized*(max - min) + min;
}

inline float toUnipolarVoltage(float normalized) {
  return scaleToRange(normalized, 0.0f, 10.0f);
}

inline float toUnipolarVoltage(bool state) {
  return state ? 10.0f : 0.0f;
}
