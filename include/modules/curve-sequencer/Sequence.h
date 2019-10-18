#pragma once

#include "StepExecutor.h"
#include "modules/components/Latch.h"

#include <memory>

namespace dhe {
namespace curve_sequencer {
  template <typename C, typename S = StepExecutor<C>> class Sequence {
  public:
    Sequence(C &controls, int stepCount, S *stepExecutor) :
        controls{controls}, stepIndexMask{stepCount - 1}, stepExecutor{stepExecutor} {}

    Sequence(C &controls, int stepCount) : Sequence(controls, stepCount, new S(controls)) {}

    void execute(float sampleTime) {
      runLatch.clock(controls.isRunning());
      gateLatch.clock(controls.gate());

      auto const selectionStart = controls.selectionStart();

      if (!isActive) {
        if (gateLatch.isRise()) {
          isActive = true;
          activeStep = selectionStart;
        } else {
          return;
        }
      }

      auto const selectionLength = controls.selectionLength();
      auto const selectionEnd = selectionStart + selectionLength - 1;

      for (int i = activeStep; i <= selectionEnd; i++) {
        activeStep = i & stepIndexMask;
        if (stepExecutor->execute(activeStep, gateLatch, sampleTime)) {
          return;
        }
      }
      isActive = false;
    }

  private:
    bool isActive{};
    int activeStep{};
    Latch runLatch{};
    Latch gateLatch{};
    C &controls;
    int const stepIndexMask;
    std::unique_ptr<S> stepExecutor;
  };
} // namespace curve_sequencer
} // namespace dhe
