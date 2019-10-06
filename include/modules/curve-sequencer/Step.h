#pragma once

#include "modules/components/Latch.h"

namespace dhe {
namespace curve_sequencer {
  struct Step {
    enum class State { Running, Finished };
    enum class Mode { Rise, Fall, Edge, High, Low, Skip, Duration, Count };

    virtual auto isAvailable() const -> bool = 0;
    virtual auto process(Latch const &gateLatch, float sampleTime) -> State = 0;
    virtual ~Step() = default;
  };
} // namespace curve_sequencer
} // namespace dhe
