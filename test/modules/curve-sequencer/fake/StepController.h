#pragma once

#include "components/Latch.h"
#include "modules/curve-sequencer/StepEvent.h"

#include <functional>

namespace test {
namespace curve_sequencer {
  namespace fake {
    using dhe::Latch;
    using dhe::curve_sequencer::StepEvent;

    struct StepController {
      std::function<void(int)> enter{[](int s) { throw "StepController.enter(step)"; }};
      std::function<StepEvent(Latch const &, float)> execute{
          [](Latch const &l, int s) -> StepEvent { throw "StepController.execute(gate,time)"; }};
      std::function<void()> exit{[]() { throw "StepController.exit()"; }};
    };
  } // namespace fake
} // namespace curve_sequencer
} // namespace test
