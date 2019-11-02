#pragma once

#include "Mode.h"
#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {
  class IdleMode : public Mode {
  public:
    auto isTerminal() const -> bool override { return true; }
    auto execute(dhe::Latch const &runLatch, dhe::Latch const &gateLatch, int /*step*/, float /*sampleTime*/) const
        -> Id override {
      if (runLatch.isFall()) {
        return Id::Paused;
      }
      return gateLatch.isRise() ? Id::Advancing : Id::Idle;
    };
  };
}; // namespace curve_sequencer
} // namespace dhe
