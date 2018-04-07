#pragma once

#include <cmath>

namespace DHE {
struct Wheel {
  const float two_pi = 2.f*std::acos(-1.f);
  const std::function<float()> radius;
  const std::function<float()> speed;
  float phase{0.f};

  Wheel(std::function<float()> radius, std::function<float()> speed)
      : radius{std::move(radius)}, speed{std::move(speed)} {}

  void step() {
    phase += speed();
    if (phase > 1.f) phase -= 1.f;
    if (phase < 0.f) phase += 1.f;
  }

  float x() const {
    return radius()*std::cos(two_pi*phase);
  }

  float y() const {
    return radius()*std::sin(two_pi*phase);
  }
};


}
