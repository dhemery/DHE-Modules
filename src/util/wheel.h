#pragma once

#include <cmath>

namespace DHE {
struct Wheel {
  const float two_pi = 2.f*std::acos(-1.f);
  float phase{0.f};

  void advance(float increment) {
    phase += increment;
    if (phase > 1.f) {
      phase -= 1.f;
    } else if (phase < 0.f) {
      phase += 1.f;
    }
  }

  float x(float radius) const {
    return radius*std::cos(two_pi*phase);
  }

  float y(float radius) const {
    return radius*std::sin(two_pi*phase);
  }
};
}
