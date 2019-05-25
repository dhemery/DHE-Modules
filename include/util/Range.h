#pragma once

namespace dhe {

static inline auto scale(float proportion, float lowerBound, float upperBound) -> float {
  return proportion * (upperBound - lowerBound) + lowerBound;
}

class Range {
public:
  constexpr Range(float lowerBound, float upperBound) noexcept : lowerBound(lowerBound), upperBound(upperBound) {}

  auto size() const -> float { return upperBound - lowerBound; }

  auto scale(float proportion) const -> float { return dhe::scale(proportion, lowerBound, upperBound); }

  auto scale(bool state) const -> float { return state ? upperBound : lowerBound; }

  auto normalize(float member) const -> float { return (member - lowerBound) / size(); }

  auto clamp(float f) const -> float {
    if (f < lowerBound) {
      return lowerBound;
    }
    if (f > upperBound) {
      return upperBound;
    }
    return f;
  }

  const float lowerBound;
  const float upperBound;
};
} // namespace dhe
