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
  };

  template <typename C, typename S = StepExecutor<C>> class Sequence {
  public:
    Sequence(C &controls, int stepCount, S *stepExecutor) :
        controls{controls}, maxStepIndex{stepCount - 1}, stepExecutor{stepExecutor} {}

    Sequence(C &controls, int stepCount) : Sequence(controls, stepCount, new S(controls)) {}

    void process(float sampleTime) {
      runLatch.clock(controls.isRunning());
      gateLatch.clock(controls.gate());
    }

  private:
    Latch runLatch{};
    Latch gateLatch{};
    C &controls;
    int const maxStepIndex;
    std::unique_ptr<S> stepExecutor;
  };
} // namespace curve_sequencer
} // namespace dhe
