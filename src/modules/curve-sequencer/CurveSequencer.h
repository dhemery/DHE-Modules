#pragma once

#include "StepEvent.h"
#include "components/latch.h"

namespace dhe {
namespace curve_sequencer {

template <typename Controls, typename StepSelector, typename StepController>
class CurveSequencer {
public:
  CurveSequencer(Controls &controls, StepSelector &stepSelector,
                 StepController &stepController)
      : controls{controls}, stepSelector{stepSelector}, stepController{
                                                            stepController} {}

  void execute(float sampleTime) {
    // Process the latches even if not running. This ensures that we detect and
    // react to edges that happen on the same sample when RUN rises.
    gateLatch.clock(controls.isGated());
    resetLatch.clock(controls.isReset());

    // Reset even if not running.
    if (resetLatch.is_rise()) {
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
    if (resetLatch.is_high()) {
      // If RESET is high while idle, copy the input voltage to the output port.
      controls.output(controls.input());
    }
    if (gateLatch.is_rise()) {
      // Consume the edge, so that the first step doesn't immediately exit if it
      // is set to advance on gate change.
      gateLatch.clock(gateLatch.is_high());
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
