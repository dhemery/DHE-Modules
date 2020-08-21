#pragma once

namespace dhe {

static inline auto scale(float proportion, float lower_bound, float upperBound)
    -> float {
  return proportion * (upperBound - lower_bound) + lower_bound;
}

static inline auto clamp(float value, float lower_bound, float upperBound)
    -> float {
  if (value < lower_bound) {
    return lower_bound;
  }
  if (value > upperBound) {
    return upperBound;
  }
  return value;
}

class Range {
public:
  constexpr Range(float lower_bound, float upper_bound) noexcept
      : lower_{lower_bound}, upper_{upper_bound} {}

  auto lower_bound() const -> float { return lower_; }

  auto upper_bound() const -> float { return upper_; }

  auto size() const -> float { return upper_ - lower_; }

  auto scale(float proportion) const -> float {
    return dhe::scale(proportion, lower_, upper_);
  }

  auto scale(bool state) const -> float { return state ? upper_ : lower_; }

  auto normalize(float scaled) const -> float {
    return (scaled - lower_) / size();
  }

  auto clamp(float scaled) const -> float {
    return dhe::clamp(scaled, lower_, upper_);
  }

private:
  float const lower_;
  float const upper_;
};
} // namespace dhe
