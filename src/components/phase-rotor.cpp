#include "components/phase-rotor.h"
#include "components/cxmath.h"

#include <cmath>

namespace dhe {
void PhaseRotor::advance(float delta) {
  phase_ += delta;
  phase_ -= std::trunc(phase_);
}

auto PhaseRotor::sin(float offset) const -> float {
  return std::sin(phase_ * tau + offset);
}

auto PhaseRotor::cos(float offset) const -> float {
  return std::cos(phase_ * tau + offset);
}

} // namespace dhe
