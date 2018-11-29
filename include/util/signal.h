#pragma once

#include "range.h"

namespace DHE {
namespace Signal {
constexpr auto unipolar_range = Range{0.f, 10.f};
constexpr auto bipolar_range = Range{-5.f, 5.f};
constexpr auto gain_range = Range{0.f, 2.0f};

inline auto range(bool is_uni) -> const Range & {
  return is_uni ? unipolar_range : bipolar_range;
}

inline auto gain(float rotation) -> float { return gain_range.scale(rotation); }

} // namespace Signal
} // namespace DHE
