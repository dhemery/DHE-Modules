#pragma once

#include "SequenceMode.h"
#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {
  template <typename Controls, typename StepSelector, typename StepController> class CurveSequencer {
  public:
    CurveSequencer(Controls &controls, StepSelector &stepSelector, StepController &generating) :
        controls{controls},
        stepSelector{stepSelector},
        stepController{generating} {}

    void execute(float sampleTime) {
      resetLatch.clock(controls.isReset());
      if (resetLatch.isRise()) {
        enter(SequenceMode::Idle);
      }

      gateLatch.clock(controls.isGated());

      if (!controls.isRunning()) {
        return;
      }

      do {
        auto const next = executeMode(sampleTime);
        if (next == mode) {
          return;
        }
        enter(next);
      } while (mode != SequenceMode::Idle);
    }

  private:
    auto executeMode(float sampleTime) -> SequenceMode {
      switch (mode) {
      case SequenceMode::Idle:
        if (gateLatch.isRise()) {
          step = stepSelector.first(gateLatch);
          return step >= 0 ? SequenceMode::Generating : SequenceMode::Idle;
        }
        if (resetLatch.isHigh()) {
          controls.output(controls.input());
        }
        return SequenceMode::Idle;
      case SequenceMode::Advancing:
        step = stepSelector.successor(step, gateLatch, controls.isLooping());
        return step >= 0 ? SequenceMode::Generating : SequenceMode::Idle;
      case SequenceMode::Generating:
        return stepController.execute(gateLatch, sampleTime);
      default:
        return SequenceMode::Idle;
      }
    }

    void enter(SequenceMode incomingMode) {
      switch (mode) {
      case SequenceMode::Generating:
        stepController.exit();
        break;
      default:
        break;
      }

      gateLatch.clock(gateLatch.isHigh()); // To remove the edge

      mode = incomingMode;
      switch (mode) {
      case SequenceMode::Generating:
        stepController.enter(step);
        return;
      case SequenceMode::Idle:
      case SequenceMode::Advancing:
        return;
      }
    }

    int step{0};
    Latch gateLatch{};
    Latch resetLatch{};
    SequenceMode mode{SequenceMode::Idle};
    Controls &controls;
    StepSelector &stepSelector;
    StepController &stepController;
  };
} // namespace curve_sequencer
} // namespace dhe
