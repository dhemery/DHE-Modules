#pragma once

#include "range.h"
#include "sigmoid.h"

namespace DHE {

namespace Durations {
static constexpr auto short_range = Range{0.001f, 1.f};
static constexpr auto medium_range = Range{0.01f, 10.f};
static constexpr auto long_range = Range{0.1f, 100.f};
} // namespace Durations

inline auto duration(float rotation, const Range &range) -> float {
  auto tapered = Sigmoid::j_taper(rotation, 0.8f);
  return range.scale(tapered);
} // namespace Duration

inline auto duration(float rotation) -> float {
  return duration(rotation, Durations::medium_range);
}
} // namespace DHE
