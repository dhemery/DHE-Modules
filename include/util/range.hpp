#pragma once

namespace DHE {

inline float scale(float proportion, float lower_bound, float upper_bound) {
  return proportion * (upper_bound - lower_bound) + lower_bound;
}

struct Range {
  float const lower_bound_;
  float const upper_bound_;

  constexpr Range(float lower_bound, float upper_bound) noexcept
      : lower_bound_(lower_bound), upper_bound_(upper_bound) {}

  float scale(float proportion) const {
    return DHE::scale(proportion, lower_bound_, upper_bound_);
  }

  float scale(bool state) const { return state ? upper_bound_ : lower_bound_; }

  float normalize(float member) const {
    return (member - lower_bound_) / (upper_bound_ - lower_bound_);
  }

  float clamp(float f) const {
    if (f < lower_bound_)
      return lower_bound_;
    if (f > upper_bound_)
      return upper_bound_;
    return f;
  }
};
} // namespace DHE
