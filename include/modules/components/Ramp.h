#pragma once

#include <algorithm>

namespace dhe {
class Ramp {
public:
  explicit Ramp(float phase = 0.F) : accumulated{std::min(phase, 1.F)}, completed{phase >= 1.F} {};

  void advance(float delta) {
    auto const phase = accumulated + delta;
    accumulated = std::min(phase, 1.F);
    completed = phase >= 1.F;
  }

  auto phase() const -> float { return accumulated; }
  auto isComplete() const -> bool { return completed; }

private:
  float accumulated{0.F};
  bool completed{false};
};

} // namespace dhe
