#pragma once

#include "modules/components/Latch.h"

#include <memory>

namespace dhe {
namespace curve_sequencer {
  static auto constexpr generateModeCount = 7;
  static auto constexpr generateDefaultMode = 6;
  static auto constexpr sustainModeCount = 6;
  static auto constexpr sustainDefaultMode = 5;

  template <typename C> class StepExecutor {
  public:
    explicit StepExecutor(C &controls) {}

    auto process(int stepIndex, Latch const &gateLatch, float sampleTime) -> bool { return false; }
  };

  template <typename C, typename S = StepExecutor<C>> class Sequence {
  public:
    Sequence(C &controls, int stepCount, S *stepExecutor) :
        controls{controls}, stepIndexMask{stepCount - 1}, stepExecutor{stepExecutor} {}

    Sequence(C &controls, int stepCount) : Sequence(controls, stepCount, new S(controls)) {}

    void process(float sampleTime) {
      runLatch.clock(controls.isRunning());
      gateLatch.clock(controls.gate());

      auto const selectionStart = controls.selectionStart();

      if (!isActive || gateLatch.isRise()) {
        isActive = true;
        activeStep = selectionStart;
      }

      auto const selectionLength = controls.selectionLength();
      auto const selectionEnd = selectionStart + selectionLength - 1;

      for (int i = activeStep; i <= selectionEnd; i++) {
        activeStep = i & stepIndexMask;
        if (stepExecutor->process(activeStep, gateLatch, sampleTime)) {
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
