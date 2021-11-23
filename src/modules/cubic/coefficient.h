#pragma once

#include "components/range.h"
#include "signals/ranged.h"

namespace dhe {
namespace cubic {
namespace internal {
static auto constexpr coefficient_range = Range{-2.F, 2.F};

struct CoefficientQuantity {
  static auto constexpr default_value = 0.F;
  static auto constexpr &range = coefficient_range;
  static auto constexpr &display_range = coefficient_range;
  static auto constexpr unit = "";
};
} // namespace internal

struct Coefficient : LinearKnob<internal::CoefficientQuantity> {};

} // namespace cubic
} // namespace dhe
