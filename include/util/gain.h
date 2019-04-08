#pragma once

#include "util/range.h"

namespace DHE {
namespace Gain {

constexpr auto range = Range{0.f, 2.f};

inline auto multiplier(float gain_amount) -> float {
  return range.scale(gain_amount);
}

} // namespace Gain
} // namespace DHE
