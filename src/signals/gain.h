#pragma once

#include "components/range.h"
#include "ranged.h"

namespace dhe {
namespace internal {
namespace attenuator {
static auto constexpr range = Range{0.F, 1.F};
static auto constexpr display_range = Range{0.F, 100.F};

struct Quantity {
  static auto constexpr default_value = 100.F;
  static auto constexpr &display_range = attenuator::display_range;
  static auto constexpr &range = attenuator::range;
  static auto constexpr unit = "%";
};
} // namespace attenuator

namespace attenuverter {
static auto constexpr range = Range{-1.F, 1.F};
static auto constexpr display_range = Range{-100.F, 100.F};

struct Quantity {
  static auto constexpr default_value = 0.F;
  static auto constexpr &display_range = attenuverter::display_range;
  static auto constexpr &range = attenuverter::range;
  static auto constexpr unit = "%";
};
} // namespace attenuverter

namespace gain {

static auto constexpr range = Range{0.F, 2.F};
static auto constexpr display_range = Range{0.F, 200.F};

struct Quantity {
  static auto constexpr default_value = 100.F;
  static auto constexpr &display_range = gain::display_range;
  static auto constexpr &range = gain::range;
  static auto constexpr unit = "%";
};
} // namespace gain
} // namespace internal

struct Attenuator : LinearKnob<internal::attenuator::Quantity> {};

struct Attenuverter : LinearKnob<internal::attenuverter::Quantity> {};

struct Gain : LinearKnob<internal::gain::Quantity> {};

using Percentage = Attenuator;
} // namespace dhe
