#pragma once

#include "ranged.h"

namespace dhe {
namespace internal {
namespace phase {

static auto constexpr range_radians = Range{-pi, pi};
static auto constexpr range_degrees = Range{-180.F, 180.F};

struct Quantity {
  static auto constexpr default_value = 0.F;
  static auto constexpr &display_range = phase::range_degrees;
  static auto constexpr &range = phase::range_radians;
  static auto constexpr unit = "˚";
};
} // namespace phase
} // namespace internal

struct Phase : LinearKnob<internal::phase::Quantity> {};
} // namespace dhe
