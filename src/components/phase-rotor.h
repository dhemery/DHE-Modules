#pragma once
#include <cmath>

namespace dhe {
class PhaseRotor {
public:
  void advance(float delta) {
    phase_ += delta;
    phase_ -= std::trunc(phase_);
  }

  static constexpr auto pi{3.14159265359F};
  static constexpr auto two_pi{2.F * pi};

  auto angle(float offset = 0.F) const -> float {
    return two_pi * (phase_ + offset);
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
