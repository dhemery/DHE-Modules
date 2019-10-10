#pragma once

#include "modules/components/Latch.h"
#include "modules/curve-sequencer/SequenceControls.h"
#include "modules/curve-sequencer/Step.h"

#include <iostream>
#include <memory>
#include <vector>

namespace dhe {
namespace curve_sequencer {

  /**
   * Controls the sequence in a CurveSequencer module.
   */
  class Sequence {
  public:
    Sequence(SequenceControls &controls, std::vector<std::unique_ptr<Step>> &steps, int stepCount) :
        controls{controls}, steps{steps}, stepIndexMask{stepCount - 1} {}

    void process(float sampleTime);

  private:
    auto indexOfFirstAvailableStep() const -> int;
    auto indexOfSuccessorStep() const -> int;

    Latch runLatch{};
    Latch gateLatch{};
    int activeStepIndex{-1};
    SequenceControls &controls;
    std::vector<std::unique_ptr<Step>> &steps;
    int const stepIndexMask;
  };
} // namespace curve_sequencer
} // namespace dhe
