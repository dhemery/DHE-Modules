#pragma once
#include <cmath>

// TODO: Make this class immutable?
namespace dhe {
class Rotor {
public:
  void advance(float delta, float offset = 0.f) {
    this->offset = offset;
    phase += delta;
    phase -= std::trunc(phase);
  }

  auto angle() const -> float { return twoPi * (phase + offset); }
  auto radius() const -> float { return std::sin(angle()); }
  auto x() const -> float { return std::cos(angle()); }
  auto y() const -> float { return std::sin(angle()); }

private:
  float const twoPi{2.f * std::acos(-1.f)};
  float phase{0.f};
  float offset{0.f};
};
} // namespace dhe
