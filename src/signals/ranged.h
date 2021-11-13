#pragma once

#include "components/cxmath.h"
#include "components/range.h"

namespace dhe {
template <typename T> struct RangedFloat {
  static auto constexpr range() -> Range { return Range{T::min, T::max}; }

  static auto constexpr scale(float normalized) -> float {
    return cx::scale(normalized, T::min, T::max);
  }

  static auto constexpr normalize(float scaled) -> float {
    return cx::normalize(scaled, T::min, T::max);
  }
};

} // namespace dhe
