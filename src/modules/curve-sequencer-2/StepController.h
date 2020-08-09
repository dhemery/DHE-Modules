#pragma once

#include "TriggerMode.h"
#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "components/Range.h"
#include "modules/curve-sequencer/StepEvent.h"

namespace dhe {
namespace curve_sequencer_2 {

  using dhe::Latch;
  using dhe::PhaseTimer;
  using dhe::curve_sequencer::StepEvent;

  template <typename Controls> class StepController {
  public:
    StepController(Controls &controls, PhaseTimer &timer) : controls{controls}, timer{timer} {}

    void enter(int step) {
      currentStep = step;
      reset();
      controls.showProgress(currentStep, 0.F);
    }

    auto execute(Latch const &gateLatch, float sampleTime) -> StepEvent {
      if (!interrupted(gateLatch)) {
        generate(sampleTime);
        if (!completed()) {
          return StepEvent::Generated;
        }
      }
      exit();
      return StepEvent::Completed;
    }

    void exit() { controls.showInactive(currentStep); }

  private:
    auto completed() const -> bool { return !timer.inProgress() && controls.advanceOnEndOfCurve(currentStep); }

    auto duration() const -> float { return controls.duration(currentStep); }

    void generate(float sampleTime) const {
      timer.advance(sampleTime / duration());
      controls.output(controls.endLevel(currentStep));
    }

    auto interrupted(Latch const &gateLatch) const -> bool {
      return controls.interruptOnTrigger(currentStep) && isTriggered(controls.triggerMode(currentStep), gateLatch);
    };

    auto level() const -> float { return controls.level(currentStep); }

    void reset() { timer.reset(); }

    auto taper(float input) const -> float {
      auto const curvature = controls.curvature(currentStep);
      auto const taper = controls.taper(currentStep);
      return taper->apply(input, curvature);
    }

    int currentStep{0};
    float startVoltage{0.F};
    Controls &controls;
    PhaseTimer &timer;
  }; // namespace curve_sequencer
} // namespace curve_sequencer_2
} // namespace dhe
