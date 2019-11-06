#pragma once

#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {

  enum class StepMode { High, Low, Calm, Skip, Generate };

  static inline auto isActive(StepMode mode, dhe::Latch const &gate) -> bool {
    switch (mode) {
    case StepMode::High:
      return gate.isHigh();
    case StepMode::Low:
      return gate.isLow();
    case StepMode::Calm:
      return !gate.isEdge();
    case StepMode::Generate:
      return true;
    case StepMode::Skip:
    default:
      return false;
    }
  }
} // namespace curve_sequencer
} // namespace dhe
