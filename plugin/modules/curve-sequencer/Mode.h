#pragma once

#include <components/Latch.h>

namespace dhe {
namespace curve_sequencer {

  class Mode {
  public:
    enum class Id { Paused, Idle, Advancing, Generating, Sustaining };

    virtual auto isTerminal() const -> bool = 0;
    virtual auto execute(Latch const &runLatch, Latch const &gateLatch, int step, float sampleTime) const -> Id = 0;
  };
}; // namespace curve_sequencer
} // namespace dhe