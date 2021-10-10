#pragma once

#include "components/latch.h"

namespace dhe {
namespace truth {
enum class GateMode {
  Rise,
  Fall,
  Edge,
  High,
  Low,
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
