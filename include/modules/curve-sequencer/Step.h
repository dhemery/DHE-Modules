#pragma once

#include "modules/components/Latch.h"

namespace dhe {
namespace curve_sequencer {
  class Step {
  public:
    virtual auto isAvailable() const -> bool = 0;
    virtual void process(Latch const &gateLatch, float sampleTime) = 0;
    virtual ~Step() = default;
  };
} // namespace curve_sequencer
} // namespace dhe
