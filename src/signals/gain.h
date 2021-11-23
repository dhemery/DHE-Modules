#pragma once

#include "components/range.h"
#include "ranged.h"

namespace dhe {
namespace internal {
static auto constexpr attenuator_range = Range{0.F, 1.F};
static auto constexpr attenuator_display_range = Range{0.F, 100.F};
static auto constexpr attenuverter_range = Range{-1.F, 1.F};
static auto constexpr attenuverter_display_range = Range{-100.F, 100.F};
static auto constexpr gain_range = Range{0.F, 2.F};
static auto constexpr gain_display_range = Range{0.F, 200.F};

struct AttenuatorQuantity {
  static auto constexpr default_value = 100.F;
  static auto constexpr &display_range = attenuator_display_range;
  static auto constexpr &range = attenuator_range;
  static auto constexpr unit = "%";
};

struct AttenuverterQuantity {
  static auto constexpr default_value = 0.F;
  static auto constexpr &display_range = attenuverter_display_range;
  static auto constexpr &range = attenuverter_range;
  static auto constexpr unit = "%";
};

struct GainQuantity {
  static auto constexpr default_value = 100.F;
  static auto constexpr &display_range = gain_display_range;
  static auto constexpr &range = gain_range;
  static auto constexpr unit = "%";
};
} // namespace internal

struct Attenuator : LinearKnob<internal::AttenuatorQuantity> {};

struct Attenuverter : LinearKnob<internal::AttenuverterQuantity> {};

struct Gain : LinearKnob<internal::GainQuantity> {};

using Percentage = Attenuator;
} // namespace dhe
