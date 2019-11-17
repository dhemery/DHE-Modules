#pragma once

#include "StepEvent.h"
#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {

  template <typename Controls, typename StepSelector, typename StepController> class CurveSequencer {
  public:
    CurveSequencer(Controls &controls, StepSelector &stepSelector, StepController &generating) :
        controls{controls}, stepSelector{stepSelector}, stepController{generating} {}

    void execute(float sampleTime) {
      resetLatch.clock(controls.isReset());

      if (resetLatch.isRise()) {
        becomeIdle();
      }

      gateLatch.clock(controls.isGated());

      if (!controls.isRunning()) {
        return;
      }

      if (step >= 0) {
        generate(sampleTime);
      } else {
        idle(sampleTime);
      }
    }

  private:
    void becomeIdle() {
      if (step >= 0) {
        stepController.exit();
      }
      step = -1;
    }

    void idle(float sampleTime) {
      if (resetLatch.isHigh()) {
        controls.output(controls.input());
      }
      if (gateLatch.isRise()) {
        becomeActive(sampleTime);
      }
    }

    void generate(float sampleTime) {
      auto const result = stepController.execute(gateLatch, sampleTime);
      if (result == StepEvent::Completed) {
        advanceSequence();
      }
    }

    void becomeActive(float sampleTime) {
      gateLatch.clock(gateLatch.isHigh());
      step = stepSelector.first(gateLatch);
      if (step >= 0) {
        stepController.enter(step);
        generate(sampleTime);
      }
    }

    void advanceSequence() {
      gateLatch.clock(gateLatch.isHigh());
      step = stepSelector.successor(step, gateLatch, controls.isLooping());
      if (step >= 0) {
        stepController.enter(step);
      }
    }

    int step{-1};
    Latch gateLatch{};
    Latch resetLatch{};
    Controls &controls;
    StepSelector &stepSelector;
    StepController &stepController;
  }; // namespace curve_sequencer
} // namespace curve_sequencer
} // namespace dhe
