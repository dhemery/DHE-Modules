#ifndef DHE_UTIL_RANGE_H
#define DHE_UTIL_RANGE_H

namespace DHE {

struct Range {
  float min, max;

  Range(float min, float max) noexcept : min(min), max(max) {}

  static float scale(float normalized, float min, float max) {
    return normalized * (max - min) + min;
  }

  float scale(float normalized) const {
    return scale(normalized, min, max);
  }

  float scale(bool state) const {
    return state ? max : min;
  }

  float normalize(float scaled) const {
    return (scaled - min) / (max - min);
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

extern const Range NORMAL;
extern const Range BIPOLAR_NORMAL;
extern const Range UNIPOLAR_VOLTAGE;
extern const Range BIPOLAR_VOLTAGE;
}
#endif
