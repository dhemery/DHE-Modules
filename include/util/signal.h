#pragma once

#include "range.h"

namespace DHE {
namespace Signal {
constexpr auto unipolar_range = Range{0.f, 10.f};
constexpr auto bipolar_range = Range{-5.f, 5.f};

inline auto range(bool is_uni) -> const Range & {
  return is_uni ? unipolar_range : bipolar_range;
}
} // namespace Signal
} // namespace DHE
