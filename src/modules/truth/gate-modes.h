#pragma once

#include "components/latch.h"
#include <array>

namespace dhe {
namespace truth {
enum class GateMode {
  Rise,
  Fall,
  Edge,
  High,
  Low,
};

static auto constexpr gate_mode_count = static_cast<int>(GateMode::Low) + 1;

static auto constexpr gate_mode_descriptions =
    std::array<char const *, gate_mode_count>{
        "Gate rises",   "Gate falls",  "Gate rises or falls",
        "Gate is high", "Gate is low",
    };

static inline auto is_satisfied(GateMode condition, dhe::Latch const &gate)
    -> bool {
  switch (condition) {
  case GateMode::High:
  default:
    return gate.is_high();
  case GateMode::Rise:
    return gate.is_rise();
  case GateMode::Fall:
    return gate.is_fall();
  case GateMode::Edge:
    return gate.is_edge();
  case GateMode::Low:
    return gate.is_low();
  }
}
} // namespace truth
} // namespace dhe
