#pragma once

#include "modules/components/Latch.h"

#include <memory>

namespace dhe {
namespace curve_sequencer {
  template <typename C> class StepExecutor {
  public:
    explicit StepExecutor(C &controls) {}

    auto execute(int stepIndex, Latch const &gateLatch, float sampleTime) -> bool { return false; }
  };
} // namespace curve_sequencer
} // namespace dhe
