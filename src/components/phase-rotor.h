#pragma once
#include "components/cxmath.h"
#include <cmath>

namespace dhe {
class PhaseRotor {
public:
  void advance(float delta) {
    phase_ += delta;
    phase_ -= std::trunc(phase_);
  }

  constexpr auto angle(float offset = 0.F) const -> float {
    return tau * (phase_ + offset);
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
