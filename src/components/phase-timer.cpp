#include "components/phase-timer.h"
#include "components/cxmath.h"

namespace dhe {
PhaseTimer::PhaseTimer(float phase) : phase_{cx::min(phase, 1.F)} {}

void PhaseTimer::advance(float delta) { phase_ = cx::min(phase_ + delta, 1.F); }

auto PhaseTimer::in_progress() const -> bool { return phase_ < 1.F; }

auto PhaseTimer::phase() const -> float { return phase_; }

void PhaseTimer::reset() { phase_ = 0.F; }

} // namespace dhe
