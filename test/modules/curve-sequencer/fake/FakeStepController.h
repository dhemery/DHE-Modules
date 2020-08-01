#pragma once

#include "components/Latch.h"
#include "modules/curve-sequencer/StepEvent.h"

#include <functional>

namespace curve_sequencer_test {
namespace fake {
  using dhe::curve_sequencer::StepEvent;
  using dhe::Latch;

  struct StepController {
    std::function<void(int)> enterFunc;
    std::function<StepEvent(Latch const &, float)> executeFunc;
    std::function<void()> exitFunc;

    void enter(int step) { return enterFunc(step); }
    auto execute(Latch const &latch, float f) -> StepEvent { return executeFunc(latch, f); }
    void exit() { return exitFunc(); }
  };
} // namespace fake
} // namespace curvesequencertest
