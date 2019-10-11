#pragma once

#include "modules/components/Latch.h"

namespace dhe {
namespace curve_sequencer {
  class Step {
  public:
    enum class State { Active, Inactive };
    enum class Mode { Rise, Fall, Edge, High, Low, Skip, Duration, Count };

    virtual ~Step() = default;

    virtual auto index() const -> int = 0;
    virtual auto isAvailable() const -> bool = 0;
    virtual auto process(Latch const &gateLatch, float sampleTime) -> State = 0;
  };
} // namespace curve_sequencer
} // namespace dhe
