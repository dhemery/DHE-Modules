#pragma once

#include "components/latch.h"
#include "components/phase-timer.h"
#include "components/range.h"
#include "step-event.h"
#include "triggers.h"

namespace dhe {
namespace curve_sequencer {

using dhe::Latch;
using dhe::curve_sequencer::StepEvent;

template <typename Interrupter, typename Generator, typename Sustainer>
class StepController {
public:
  StepController(Interrupter &interrupter, Generator &generator,
                 Sustainer &sustainer)
      : interrupter_{interrupter}, generator_{generator}, sustainer_{
                                                              sustainer} {}

  void enter(int step) {
    current_step_ = step;
    generator_.start(step);
  }

  auto execute(Latch const &gate, float sample_time) -> StepEvent {
    if (!interrupted(gate)) {
      auto const curve_completed = generator_.generate(sample_time);
      if (!curve_completed || !sustainer_.is_done(current_step_, gate)) {
        return StepEvent::Generated;
      }
    }
    exit();
    return StepEvent::Completed;
  }

  void exit() { generator_.stop(); }

private:
  auto interrupted(Latch const &gate) const -> bool {
    return interrupter_.is_interrupted(current_step_, gate);
  };

  int current_step_{0};
  Interrupter &interrupter_;
  Generator &generator_;
  Sustainer &sustainer_;
}; // namespace curve_sequencer
} // namespace curve_sequencer
} // namespace dhe
