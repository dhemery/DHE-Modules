#pragma once

#include <algorithm>

namespace dhe {
class PhaseTimer {
public:
  PhaseTimer(float phase = 0.F) : accumulatedPhase{std::min(phase, 1.F)} {}

  void advance(float delta) { accumulatedPhase = std::min(accumulatedPhase + delta, 1.F); }

  auto isExpired() const -> bool { return accumulatedPhase >= 1.F; }

  auto phase() const -> float { return accumulatedPhase; }

  void reset() { accumulatedPhase = 0.F; }

private:
  float accumulatedPhase{};
};

} // namespace dhe
