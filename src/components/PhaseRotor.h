#pragma once
#include <cmath>

namespace dhe {
class PhaseRotor {
public:
  void advance(float delta) {
    phase_ += delta;
    phase_ -= std::trunc(phase_);
  }

  auto angle(float offset = 0.F) const -> float {
    static float const two_pi{2.F * std::acos(-1.F)};
    return two_pi * (phase_ + offset);
  }

  auto sin(float offset = 0.F) const -> float {
    return std::sin(angle(offset));
  }
  auto cos(float offset = 0.F) const -> float {
    return std::cos(angle(offset));
  }

private:
  float phase_{0.F};
};

} // namespace dhe
