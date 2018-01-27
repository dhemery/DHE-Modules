#ifndef DHE_UTIL_FUNCTIONS_H
#define DHE_UTIL_FUNCTIONS_H

#include <cmath>
#include <functional>

#include "range.h"

namespace DHE {

inline float sigmoid(float x, float curvature) {
  static constexpr float precision{1e-4f};
  static constexpr float maxCurvature{1.0f - precision};
  static const Range curvatureRange{-maxCurvature, maxCurvature};
  curvature = curvatureRange.clamp(curvature);
  x = BIPOLAR_NORMAL.clamp(x);
  return (x - x*curvature)/(curvature - abs(x)*2.0f*curvature + 1.0f);
}
}
#endif