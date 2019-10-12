#pragma once
#include <cmath>

namespace dhe {
class CyclicPhaseAccumulator {
public:
  auto advance(float delta) -> float {
    phase += delta;
    phase -= std::trunc(phase);
    return phase;
  }

  auto angle(float offset = 0.F) -> float {
    static float const twoPi{2.F * std::acos(-1.F)};
    return twoPi * (phase + offset);
  }

  auto sin(float offset = 0.F) -> float { return std::sin(angle(offset)); }
  auto cos(float offset = 0.F) -> float { return std::cos(angle(offset)); }

private:
  float phase{0.F};
};

} // namespace dhe
