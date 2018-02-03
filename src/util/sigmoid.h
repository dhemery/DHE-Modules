#ifndef DHE_UTIL_FUNCTIONS_H
#define DHE_UTIL_FUNCTIONS_H

#include <cmath>
#include <functional>

#include "interval.h"

namespace DHE {

inline float sigmoid(float x, float curvature) {
  static constexpr auto precision{1e-4f};
  static constexpr auto max_curvature{1.0f - precision};
  static const auto curvature_range = Interval{-max_curvature, max_curvature};

  curvature = curvature_range.clamp(curvature);
  x = BIPOLAR_NORMAL.clamp(x);

  return (x - x*curvature)/(curvature - std::abs(x)*2.0f*curvature + 1.0f);
}
}
#endif