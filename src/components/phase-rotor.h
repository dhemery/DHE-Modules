#pragma once

#include "cxmath.h"

#include <cmath>

namespace dhe {
class PhaseRotor {
public:
  void advance(float delta) {
    phase_ += delta;
    phase_ -= std::trunc(phase_);
  }

  auto sin(float offset = 0.F) const -> float {
    return std::sin(phase_ * tau + offset);
  }

  auto cos(float offset = 0.F) const -> float {
    return std::cos(phase_ * tau + offset);
  }

private:
  float phase_{0.F};
};

} // namespace dhe
