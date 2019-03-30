#pragma once

#include <cmath>

namespace DHE {
class Rotor {
public:
  void advance(float delta, float offset = 0.f) {
    this->offset = offset;
    phase += delta;
    phase -= std::trunc(phase);
  }

  auto angle() const -> float { return two_pi * (phase + offset); }
  auto radius() const -> float { return std::sin(angle()); }
  auto x() const -> float { return std::cos(angle()); }
  auto y() const -> float { return std::sin(angle()); }

private:
  float const two_pi{2.f * std::acos(-1.f)};
  float phase{0.f};
  float offset{0.f};
};
} // namespace DHE
