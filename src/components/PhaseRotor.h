#pragma once
#include <cmath>

namespace dhe {
class PhaseRotor {
public:
  void advance(float delta) {
    phase += delta;
    phase -= std::trunc(phase);
  }

  auto angle(float offset = 0.F) const -> float {
    static float const twoPi{2.F * std::acos(-1.F)};
    return twoPi * (phase + offset);
  }

  auto sin(float offset = 0.F) const -> float { return std::sin(angle(offset)); }
  auto cos(float offset = 0.F) const -> float { return std::cos(angle(offset)); }

private:
  float phase{0.F};
};

} // namespace dhe
