#pragma once

#include "StepMode.h"
#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {

  enum class StepCondition { TimerExpires, GateIsLow, GateIsHigh, GateRises, GateFalls, GateChanges };

  static inline auto isSatisfied(StepMode mode, StepCondition condition, dhe::Latch const &gate) -> bool {
    switch (condition) {
    case StepCondition::GateIsHigh:
      return gate.isHigh();
    case StepCondition::GateIsLow:
      return gate.isLow();
    case StepCondition::GateRises:
      return gate.isRise();
    case StepCondition::GateFalls:
      return gate.isFall();
    case StepCondition::GateChanges:
      return gate.isEdge();
    case StepCondition::TimerExpires:
    default:
      return mode == StepMode::Sustain;
    }
  }
} // namespace curve_sequencer
} // namespace dhe
