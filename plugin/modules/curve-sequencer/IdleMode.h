#pragma once

#include "Mode.h"
#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {
  class IdleMode : public Mode {
  public:
    auto isTerminal() const -> bool override { return true; }
    auto execute(dhe::Latch const &runLatch, dhe::Latch const &gateLatch, int step, float /*sampleTime*/) const
        -> Successor override {
      if (runLatch.isFall()) {
        return {ModeId::Paused, step};
      }
      ModeId nextMode = gateLatch.isRise() ? ModeId::Advancing : ModeId::Idle;
      return {nextMode, step};
    };
  };
}; // namespace curve_sequencer
} // namespace dhe
