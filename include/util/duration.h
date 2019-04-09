#pragma once

#include "range.h"
#include "sigmoid.h"
#include <array>

namespace DHE {

namespace Duration {

static constexpr auto short_range = Range{0.001f, 1.f};
static constexpr auto medium_range = Range{0.01f, 10.f};
static constexpr auto long_range = Range{0.1f, 100.f};

auto ranges() -> std::array<Range const *, 3> const &;
} // namespace Duration

static inline auto duration(float rotation, Range const &range) -> float {
  auto tapered = Sigmoid::j_taper(rotation, 0.8f);
  return range.scale(tapered);
}

static inline auto duration(float rotation) -> float {
  return duration(rotation, Duration::medium_range);
}
} // namespace DHE
