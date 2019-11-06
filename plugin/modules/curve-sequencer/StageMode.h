#pragma once

#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {

  enum class StageMode { High, Low, Calm, Skip, Generate };

  static inline auto isActive(StageMode mode, dhe::Latch const &gate) -> bool {
    switch (mode) {
    case StageMode::High:
      return gate.isHigh();
    case StageMode::Low:
      return gate.isLow();
    case StageMode::Calm:
      return !gate.isEdge();
    case StageMode::Generate:
      return true;
    case StageMode::Skip:
    default:
      return false;
    }
  }
} // namespace curve_sequencer
} // namespace dhe
