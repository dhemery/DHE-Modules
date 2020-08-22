#pragma once

#include "cxmath.h"

namespace dhe {
class Range {
public:
  constexpr Range(float lower_bound, float upper_bound) noexcept
      : lower_{lower_bound}, upper_{upper_bound} {}

  constexpr auto lower_bound() const -> float { return lower_; }

  constexpr auto upper_bound() const -> float { return upper_; }

  constexpr auto size() const -> float { return upper_ - lower_; }

  constexpr auto scale(float proportion) const -> float {
    return cx::scale(proportion, lower_, upper_);
  }

  constexpr auto scale(bool state) const -> float {
    return state ? upper_ : lower_;
  }

  constexpr auto normalize(float scaled) const -> float {
    return (scaled - lower_) / size();
  }

  constexpr auto clamp(float scaled) const -> float {
    return cx::clamp(scaled, lower_, upper_);
  }

  constexpr auto operator==(const Range &rhs) const -> bool {
    return lower_ == rhs.lower_ && upper_ == rhs.upper_;
  }

  constexpr auto operator!=(const Range &rhs) const -> bool {
    return !(rhs == *this);
  }

private:
  float const lower_;
  float const upper_;
};
} // namespace dhe
