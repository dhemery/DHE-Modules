#pragma once

#include "ranged.h"

namespace dhe {
namespace internal {
static auto constexpr phase_range_radians = Range{-pi, pi};
static auto constexpr phase_range_degrees = Range{-180.F, 180.F};

struct PhaseQuantity {
  static auto constexpr default_value = 0.F;
  static auto constexpr &display_range = phase_range_degrees;
  static auto constexpr &range = phase_range_radians;
  static auto constexpr unit = "˚";
};
} // namespace internal

struct Phase : LinearKnob<internal::PhaseQuantity> {};
} // namespace dhe
