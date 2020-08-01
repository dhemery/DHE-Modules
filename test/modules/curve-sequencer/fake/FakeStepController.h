#pragma once

#include "components/Latch.h"
#include "modules/curve-sequencer/StepEvent.h"

#include <functional>

namespace test {
namespace curve_sequencer {
  using dhe::Latch;
  using dhe::curve_sequencer::StepEvent;

  struct FakeStepController {
    std::function<void(int)> enter;
    std::function<StepEvent(Latch const &, float)> execute;
    std::function<void()> exit;
  };
} // namespace curve_sequencer
} // namespace test
