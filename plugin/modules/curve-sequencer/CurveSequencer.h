#pragma once

#include "SequenceMode.h"
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
      default:
      case SequenceMode::Idle:
        return idle();
      case SequenceMode::Advancing:
        return advance();
      case SequenceMode::Generating:
        return generate(sampleTime);
      }
    }

    auto advance() -> SequenceMode {
      step = stepSelector.successor(step, gateLatch, controls.isLooping());
      return step >= 0 ? SequenceMode::Generating : SequenceMode::Idle;
    }

    auto idle() -> SequenceMode {
      if (gateLatch.isRise()) {
        step = stepSelector.first(gateLatch);
        return step >= 0 ? SequenceMode::Generating : SequenceMode::Idle;
      }
      if (resetLatch.isHigh()) {
        controls.output(controls.input());
      }
      return SequenceMode::Idle;
    }

    auto generate(float sampleTime) -> SequenceMode {
      auto const result = stepController.execute(gateLatch, sampleTime);
      return (result == StepEvent::Generated) ? SequenceMode::Generating : SequenceMode::Advancing;
    }

    void enter(SequenceMode incomingMode) {
      if (mode == SequenceMode::Generating) {
        stepController.exit();
      }

      gateLatch.clock(gateLatch.isHigh()); // To remove the edge

      mode = incomingMode;

      if (mode == SequenceMode::Generating) {
        stepController.enter(step);
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
