#ifndef DHE_UTIL_RANGE_H
#define DHE_UTIL_RANGE_H

namespace DHE {

struct Range {
  float min, max;

  Range(float min, float max) noexcept : min(min), max(max) {}

  static float scale(float normalized, float min, float max) {
    return normalized*(max - min) + min;
  }

  float scale(float normalized) const {
    return scale(normalized, min, max);
  }

  float scale(bool state) const {
    return state ? max : min;
  }

  float normalize(float scaled) const {
    return (scaled - min)/(max - min);
  }

  float clamp(float scaled) const {
    if (scaled < min)
      return min;
    if (scaled > max)
      return max;
    return scaled;
  }

  float clamp_to_normal(float scaled) const {
    return normalize(clamp(scaled));
  }
};

const Range NORMAL{0.0f, 1.0f};
const Range BIPOLAR_NORMAL{-1.0f, 1.0f};
const Range UNIPOLAR_VOLTAGE{0.0f, 10.0f};
const Range BIPOLAR_VOLTAGE{-5.0f, 5.0f};
}
#endif
