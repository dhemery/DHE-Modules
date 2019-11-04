#pragma once

#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {

  enum class InterruptMode { Rise, Fall, Edge, High, Low, Skip, Duration };

  static inline auto interrupted(InterruptMode mode, dhe::Latch const &gate) -> bool {
    switch (mode) {
    case InterruptMode::Rise:
      return gate.isRise();
    case InterruptMode::Fall:
      return gate.isFall();
    case InterruptMode::Edge:
      return gate.isEdge();
    case InterruptMode::High:
      return gate.isHigh();
    case InterruptMode::Low:
      return gate.isLow();
    case InterruptMode::Duration:
      return false;
    case InterruptMode::Skip:
    default:
      return true;
    }
  }
} // namespace curve_sequencer
} // namespace dhe
