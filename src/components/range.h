#pragma once

namespace dhe {

static constexpr auto scale(float proportion, float lower_bound,
                            float upper_bound) -> float {
  return proportion * (upper_bound - lower_bound) + lower_bound;
}

static inline auto clamp(float value, float lower_bound, float upper_bound)
    -> float {
  if (value < lower_bound) {
    return lower_bound;
  }
  if (value > upper_bound) {
    return upper_bound;
  }
  return value;
}

class Range {
public:
  constexpr Range(float lower_bound, float upper_bound) noexcept
      : lower_{lower_bound}, upper_{upper_bound} {}

  constexpr auto lower_bound() const -> float { return lower_; }

  constexpr auto upper_bound() const -> float { return upper_; }

  constexpr auto size() const -> float { return upper_ - lower_; }

  constexpr auto scale(float proportion) const -> float {
    return dhe::scale(proportion, lower_, upper_);
  }

  constexpr auto scale(bool state) const -> float {
    return state ? upper_ : lower_;
  }

  constexpr auto normalize(float scaled) const -> float {
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
