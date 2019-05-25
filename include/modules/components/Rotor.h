#pragma once
#include <cmath>

// TODO: Make this class immutable?
namespace dhe {
class Rotor {
public:
  void advance(float delta, float offset = 0.F) {
    this->offset = offset;
    phase += delta;
    phase -= std::trunc(phase);
  }

  auto angle() const -> float { return twoPi * (phase + offset); }
  auto radius() const -> float { return std::sin(angle()); }
  auto x() const -> float { return std::cos(angle()); }
  auto y() const -> float { return std::sin(angle()); }

private:
  float const twoPi{2.F * std::acos(-1.F)};
  float phase{0.F};
  float offset{0.F};
};
} // namespace dhe
