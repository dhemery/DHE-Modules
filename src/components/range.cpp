#include "components/range.h"
#include "components/cxmath.h"

namespace dhe {
Range::Range(float lower_bound, float upper_bound) noexcept
    : lower_{lower_bound}, upper_{upper_bound} {}

auto Range::lower_bound() const -> float { return lower_; }

auto Range::upper_bound() const -> float { return upper_; }

auto Range::size() const -> float { return upper_ - lower_; }

auto Range::scale(float proportion) const -> float {
  return cx::scale(proportion, lower_, upper_);
}

auto Range::scale(bool state) const -> float { return state ? upper_ : lower_; }

auto Range::normalize(float scaled) const -> float {
  return cx::normalize(scaled, lower_, upper_);
}

auto Range::clamp(float scaled) const -> float {
  return cx::clamp(scaled, lower_, upper_);
}

auto Range::operator==(Range rhs) const -> bool {
  return lower_ == rhs.lower_ && upper_ == rhs.upper_;
}

auto Range::operator!=(Range rhs) const -> bool { return !(rhs == *this); }

} // namespace dhe
