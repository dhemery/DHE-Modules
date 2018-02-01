#ifndef DHE_UTIL_RANGE_H
#define DHE_UTIL_RANGE_H

namespace DHE {

struct Interval {
  const float lower_bound;
  const float upper_bound;

  constexpr Interval(float lower_bound, float upper_bound) noexcept : lower_bound(lower_bound), upper_bound(upper_bound) {}

  static float scale(float proportion, float lower_bound, float upper_bound) {
    return proportion*(upper_bound - lower_bound) + lower_bound;
  }

  float scale(float proportion) const {
    return scale(proportion, lower_bound, upper_bound);
  }

  float scale(bool state) const {
    return state ? upper_bound : lower_bound;
  }

  float normalize(float member) const {
    return (member - lower_bound)/(upper_bound - lower_bound);
  }

  float clamp(float f) const {
    if (f < lower_bound)
      return lower_bound;
    if (f > upper_bound)
      return upper_bound;
    return f;
  }
};

constexpr auto NORMAL = Interval{0.0f, 1.0f};
constexpr auto BIPOLAR_NORMAL = Interval{-1.0f, 1.0f};
constexpr auto UNIPOLAR_CV = Interval{0.0f, 10.0f};
constexpr auto BIPOLAR_CV = Interval{-5.0f, 5.0f};
}
#endif
