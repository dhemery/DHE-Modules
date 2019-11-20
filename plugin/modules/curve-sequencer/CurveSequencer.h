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
      // Process the gate latch even if not running, so that if GATE rises or falls at the same time as RUN rises, the
      // latch detects the edge.
      gateLatch.clock(controls.isGated());

      // Reset even if not running.
      resetLatch.clock(controls.isReset());
      if (resetLatch.isRise()) {
        becomeIdle();
      }

      if (!controls.isRunning()) {
        return;
      }

      if (step < 0) {
        idle();
      }
      if (step >= 0) {
        generate(sampleTime);
      }
    }

  private:
    void becomeIdle() {
      if (step >= 0) {
        stepController.exit();
      }
      step = -1;
    }

    void idle() {
      if (resetLatch.isHigh()) {
        // If RESET is high while idle, copy the input voltage to the output port
        controls.output(controls.input());
      }
      if (gateLatch.isRise()) {
        // Consume the edge, so that the first step doesn't immediately exit if it is set to advance on gate change.
        gateLatch.clock(gateLatch.isHigh());
        startSequence();
      }
    }

    void generate(float sampleTime) {
      auto const result = stepController.execute(gateLatch, sampleTime);
      if (result == StepEvent::Completed) {
        advanceSequence();
      }
    }

    void startSequence() {
      step = stepSelector.first();
      if (step >= 0) {
        stepController.enter(step);
      }
    }

    void advanceSequence() {
      step = stepSelector.successor(step, controls.isLooping());
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
