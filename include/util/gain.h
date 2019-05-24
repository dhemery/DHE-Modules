#pragma once

#include "util/range.h"

namespace dhe {
namespace Gain {

  static constexpr auto range = Range{0.f, 2.f};

  static inline auto multiplier(float gainAmount) -> float { return range.scale(gainAmount); }

} // namespace Gain
} // namespace dhe
