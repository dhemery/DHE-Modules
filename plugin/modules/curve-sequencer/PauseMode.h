#pragma once

#include "Mode.h"
#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {
  class PauseMode : public Mode {
  public:
    auto isTerminal() const -> bool override { return true; }
    auto execute(dhe::Latch const &runLatch, dhe::Latch const & /*gateLatch*/, int /*step*/, float /*sampleTime*/) const
        -> Id override {
      return runLatch.isRise() ? Id::Idle : Id::Paused;
    };
  };
}; // namespace curve_sequencer
} // namespace dhe
