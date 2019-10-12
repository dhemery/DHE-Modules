#pragma once

#include <algorithm>

namespace dhe {
class OneShotPhaseAccumulator {
public:
  enum class State { Incomplete, Complete };

  void advance(float delta) {
    auto const phase = accumulated + delta;
    accumulated = std::min(phase, 1.F);
    if (phase >= 1.F) {
      completed = State::Complete;
    }
  }

  auto phase() const -> float { return accumulated; }

  auto state() const -> State { return completed; }

  void reset() {
    accumulated = 0.F;
    completed = State::Incomplete;
  }

private:
  float accumulated{0.F};
  State completed{State::Incomplete};
};

} // namespace dhe
