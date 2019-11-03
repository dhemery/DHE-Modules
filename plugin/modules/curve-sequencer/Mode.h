#pragma once

#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {
  enum class ModeId { Paused, Idle, Advancing, Generating, Sustaining };

  struct Successor {
    ModeId modeId;
    int step;
  };

  class Mode {
  public:
    virtual auto isTerminal() const -> bool = 0;
    virtual auto execute(Latch const &runLatch, Latch const &gateLatch, int step, float sampleTime) const
        -> Successor = 0;
  };
}; // namespace curve_sequencer
} // namespace dhe
