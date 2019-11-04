#pragma once

#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {

  enum class RunMode { High, Low, Calm, Skip, Generate };

  static inline auto isRunning(RunMode mode, dhe::Latch const &gate) -> bool {
    switch (mode) {
    case RunMode::High:
      return gate.isHigh();
    case RunMode::Low:
      return gate.isLow();
    case RunMode::Calm:
      return !gate.isEdge();
    case RunMode::Generate:
      return true;
    case RunMode::Skip:
    default:
      return false;
    }
  }
} // namespace curve_sequencer
} // namespace dhe
