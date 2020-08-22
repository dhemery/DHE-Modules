#pragma once

#include "components/cxmath.h"

namespace dhe {
class PhaseTimer {
public:
  constexpr PhaseTimer(float phase = 0.F) : phase_{cx::min(phase, 1.F)} {}

  void advance(float delta) { phase_ = cx::min(phase_ + delta, 1.F); }

  constexpr auto in_progress() const -> bool { return phase_ < 1.F; }

  constexpr auto phase() const -> float { return phase_; }

  void reset() { phase_ = 0.F; }

private:
  float phase_{};
};

} // namespace dhe
