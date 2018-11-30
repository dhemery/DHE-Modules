#pragma once

#include "range.h"
#include "sigmoid.h"

namespace DHE {

inline auto duration(float rotation) -> float {
  static constexpr auto medium_range = Range{0.01f, 10.f};
  auto tapered = Sigmoid::j_taper(rotation, 0.8f);
  return medium_range.scale(tapered);
}

inline auto duration(float rotation, int selection) -> float {
  static constexpr auto short_range = Range{0.001f, 1.f};
  static constexpr auto medium_range = Range{0.01f, 10.f};
  static constexpr auto long_range = Range{0.1f, 100.f};

  auto tapered = Sigmoid::j_taper(rotation, 0.8f);
  auto range =
      selection > 1 ? long_range : selection < 1 ? short_range : medium_range;
  return range.scale(tapered);
} // namespace Duration
} // namespace DHE
