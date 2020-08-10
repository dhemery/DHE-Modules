#pragma once

#include "TriggerMode.h"
#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "components/Range.h"
#include "modules/curve-sequencer/StepEvent.h"

namespace dhe {
namespace curve_sequencer_2 {

  using dhe::Latch;
  using dhe::curve_sequencer::StepEvent;

  template <typename Interrupter, typename Generator, typename Sustainer> class StepController {
  public:
    StepController(Interrupter &interrupter, Generator &generator, Sustainer &sustainer) :
        interrupter{interrupter}, generator{generator}, sustainer{sustainer} {}

    void enter(int step) {
      currentStep = step;
      generator.start(step);
    }

    auto execute(Latch const &gateLatch, float sampleTime) -> StepEvent {
      if (!interrupted(gateLatch)) {
        auto const curveCompleted = generator.generate(sampleTime);
        if (!curveCompleted || !sustainer.isDone(currentStep, gateLatch)) {
          return StepEvent::Generated;
        }
      }
      exit();
      return StepEvent::Completed;
    }

    void exit() { generator.stop(); }

  private:
    auto interrupted(Latch const &gateLatch) const -> bool {
      return interrupter.isInterrupted(currentStep, gateLatch);
    };

    int currentStep{0};
    Interrupter &interrupter;
    Generator &generator;
    Sustainer &sustainer;
  }; // namespace curve_sequencer
} // namespace curve_sequencer_2
} // namespace dhe
