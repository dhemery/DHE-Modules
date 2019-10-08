#pragma once

#include "modules/components/Latch.h"
#include "modules/curve-sequencer/SequenceControls.h"
#include "modules/curve-sequencer/Step.h"

#include <memory>
#include <vector>

namespace dhe {
namespace curve_sequencer {

  /**
   * Controls the sequence in a CurveSequencer module.
   */
  struct Sequence {
    Sequence(SequenceControls &controls, std::vector<std::unique_ptr<Step>> &steps) :
        controls{controls}, steps{steps} {}

    void process(float sampleTime);

  private:
    void start(float sampleTime);

    SequenceControls &controls;
    std::vector<std::unique_ptr<Step>> &steps;
    Latch runLatch{};
    Latch gateLatch{};
  };
} // namespace curve_sequencer
} // namespace dhe
