#pragma once

#include <cmath>

namespace DHE {
struct Wheel {
  const float two_pi = 2.f*std::acos(-1.f);
  float phase{0.f};

  void advance(float increment) {
    phase += increment;
    phase -= std::trunc(phase); // Reduce phase to (-1, 1) to avoid eventual overflow
  }

  float x(float radius) const {
    return radius*std::cos(two_pi*phase);
  }

  float y(float radius) const {
    return radius*std::sin(two_pi*phase);
  }
};
}
