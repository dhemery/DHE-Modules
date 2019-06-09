#pragma once

namespace dhe {

static inline auto scale(float proportion, float lowerBound, float upperBound) -> float {
  return proportion * (upperBound - lowerBound) + lowerBound;
}

static inline auto clamp(float value, float lowerBound, float upperBound) -> float {
  if (value < lowerBound) {
    return lowerBound;
  }
  if (value > upperBound) {
    return upperBound;
  }
  return value;
}

class Range {
public:
  constexpr Range(float lowerBound, float upperBound) noexcept : lowerBound(lowerBound), upperBound(upperBound) {}

  auto size() const -> float { return upperBound - lowerBound; }

  auto scale(float proportion) const -> float { return dhe::scale(proportion, lowerBound, upperBound); }

  auto scale(bool state) const -> float { return state ? upperBound : lowerBound; }

  auto normalize(float member) const -> float { return (member - lowerBound) / size(); }

  auto clamp(float f) const -> float { return dhe::clamp(f, lowerBound, upperBound); }

  const float lowerBound;
  const float upperBound;
};
} // namespace dhe
