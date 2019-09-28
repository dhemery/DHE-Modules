#pragma once
#include <cmath>

namespace dhe {
class PhaseAccumulator {
public:
  void advance(float delta) {
    accumulated += delta;
    accumulated -= std::trunc(accumulated);
  }

  inline auto phase() const -> float { return accumulated; }

  inline auto angle(float offset = 0.F) -> float {
    static float const twoPi{2.F * std::acos(-1.F)};
    return twoPi * (phase() + offset);
  }

  inline auto sin(float offset = 0.F) -> float { return std::sin(angle(offset)); }
  inline auto cos(float offset = 0.F) -> float { return std::cos(angle(offset)); }

private:
  float accumulated{0.F};
};

} // namespace dhe
