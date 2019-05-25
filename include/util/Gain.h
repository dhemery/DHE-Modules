#pragma once

#include "util/Range.h"

namespace dhe {
namespace gain {

  static constexpr auto range = Range{0.f, 2.f};

  static inline auto multiplier(float gainAmount) -> float { return range.scale(gainAmount); }

} // namespace gain
} // namespace dhe
