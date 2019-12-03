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
  constexpr Range(float lowerBound, float upperBound) noexcept : lower{lowerBound}, upper{upperBound} {}

  auto lowerBound() const -> float { return lower; }

  auto upperBound() const -> float { return upper; }

  auto size() const -> float { return upper - lower; }

  auto scale(float proportion) const -> float { return dhe::scale(proportion, lower, upper); }

  auto scale(bool state) const -> float { return state ? upper : lower; }

  auto normalize(float scaled) const -> float { return (scaled - lower) / size(); }

  auto clamp(float scaled) const -> float { return dhe::clamp(scaled, lower, upper); }

private:
  float const lower;
  float const upper;
};
} // namespace dhe
