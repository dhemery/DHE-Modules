#pragma once

#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {

  enum class InterruptMode { Rise, Fall, Edge, High, Low, Skip, Duration };

  static inline auto isActive(InterruptMode mode, dhe::Latch const &gateLatch) -> bool {
    switch (mode) {
    case InterruptMode::Rise:
      return !gateLatch.isRise();
    case InterruptMode::Fall:
      return !gateLatch.isFall();
    case InterruptMode::Edge:
      return !gateLatch.isEdge();
    case InterruptMode::High:
      return !gateLatch.isHigh();
    case InterruptMode::Low:
      return !gateLatch.isLow();
    case InterruptMode::Duration:
      return true;
    case InterruptMode::Skip:
    default:
      return false;
    }
  }
} // namespace curve_sequencer
} // namespace dhe
