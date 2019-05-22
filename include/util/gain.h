#pragma once

#include "util/range.h"

namespace dhe {
namespace Gain {

static constexpr auto range = Range{0.f, 2.f};

static inline auto multiplier(float gain_amount) -> float {
  return range.scale(gain_amount);
}

} // namespace Gain
} // namespace dhe
