#pragma once

#include "components/latch.h"
#include "components/phase-timer.h"
#include "components/range.h"
#include "generator-status.h"
#include "step-selector.h"
#include "step-status.h"

namespace dhe {
namespace curve_sequencer {

using dhe::Latch;

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

  auto execute(Latch const &gate, float sample_time) -> StepStatus {
    if (!interrupted(gate)) {
      auto const generator_status = generator_.generate(sample_time);
      if ((generator_status == GeneratorStatus::Generating) ||
          !sustainer_.is_done(current_step_, gate)) {
        return StepStatus::InProgress;
      }
    }
    exit();
    return StepStatus::Completed;
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
