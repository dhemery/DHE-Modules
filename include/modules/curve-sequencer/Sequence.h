#pragma once

#include "modules/curve-sequencer/Step.h"

#include <modules/components/Latch.h>
#include <utility>
#include <vector>

namespace dhe {
namespace curve_sequencer {
  /**
   * Controls the sequence in a CurveSequencer module.
   */
  class Sequence {
  public:
    Sequence(Latch &runLatch, Latch &gateLatch, std::vector<std::unique_ptr<Step>> const &steps) :
        runLatch{runLatch}, gateLatch{gateLatch}, steps{steps} {}

    void process(float sampleTime) {
      runLatch.step();
      gateLatch.step();

      if (!runLatch.isHigh()) {
        return;
      }
      if (gateLatch.isHigh()) {
        steps[0]->process(sampleTime);
      }
    }

  private:
    Latch &runLatch;
    Latch &gateLatch;
    std::vector<std::unique_ptr<Step>> const &steps;
  };
} // namespace curve_sequencer
} // namespace dhe
