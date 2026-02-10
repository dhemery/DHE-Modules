#pragma once

#include <ostream>

namespace dhe {
class Range {
public:
  Range(float lower_bound, float upper_bound) noexcept;

  auto lower_bound() const -> float;

  auto upper_bound() const -> float;

  auto size() const -> float;

  auto scale(float proportion) const -> float;

  auto scale(bool state) const -> float;

  auto normalize(float scaled) const -> float;

  auto clamp(float scaled) const -> float;

  auto operator==(Range rhs) const -> bool;

  auto operator!=(Range rhs) const -> bool;

  friend auto operator<<(std::ostream &os, Range range) -> std::ostream & {
    os << "Range{" << range.lower_ << "," << range.upper_ << '}';
    return os;
  }

private:
  float lower_;
  float upper_;
};

} // namespace dhe
