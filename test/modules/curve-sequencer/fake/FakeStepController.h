#pragma once

#include "components/Latch.h"
#include "modules/curve-sequencer/StepEvent.h"

#include <functional>

namespace test {
namespace curve_sequencer {
  using dhe::Latch;
  using dhe::curve_sequencer::StepEvent;

  struct FakeStepController {
    std::function<void(int)> enter{[](int s) { throw "FakeStepController.enter(step)"; }};
    std::function<StepEvent(Latch const &, float)> execute{
        [](Latch const &l, int s) -> StepEvent { throw "FakeStepController.execute(gate,time)"; }};
    std::function<void()> exit{[]() { throw "FakeStepController.exit()"; }};
  };
} // namespace curve_sequencer
} // namespace test
