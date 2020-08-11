#pragma once

#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer_2 {
  enum class TriggerMode { GateRises, GateFalls, GateChanges, GateIsHigh, GateIsLow };

  static auto constexpr triggerModeCount = static_cast<int>(TriggerMode::GateIsLow) + 1;

  static inline auto isTriggered(TriggerMode mode, dhe::Latch const &gate) -> bool {
    switch (mode) {
    case TriggerMode::GateIsHigh:
    default:
      return gate.isHigh();
    case TriggerMode::GateIsLow:
      return gate.isLow();
    case TriggerMode::GateRises:
      return gate.isRise();
    case TriggerMode::GateFalls:
      return gate.isFall();
    case TriggerMode::GateChanges:
      return gate.isEdge();
    }
  }

  template <typename Controls> class Interrupter {
  public:
    Interrupter(Controls &controls) : controls{controls} {}

    auto isInterrupted(int step, Latch const &latch) -> bool {
      return controls.interruptOnTrigger(step) && isTriggered(controls.triggerMode(step), latch);
    }

  private:
    Controls &controls;
  };

  template <typename Controls> class Sustainer {
  public:
    Sustainer(Controls &controls) : controls{controls} {}

    auto isDone(int step, Latch const &latch) -> bool {
      return controls.advanceOnEndOfCurve(step) || isTriggered(controls.triggerMode(step), latch);
    }

  private:
    Controls &controls;
  };

} // namespace curve_sequencer_2
} // namespace dhe
