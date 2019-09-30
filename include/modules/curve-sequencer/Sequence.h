#pragma once

#include "modules/components/Latch.h"
#include "modules/curve-sequencer/Step.h"

#include <vector>

namespace dhe {
namespace curve_sequencer {

  /**
   * Controls the sequence in a CurveSequencer module.
   * @tparam PtrT the type of smart pointer that holds the latches and steps
   */
  class Sequence {
  public:
    Sequence(Latch &runLatch, Latch &gateLatch, std::vector<std::unique_ptr<Step>> &steps) :
        runLatch{runLatch}, gateLatch{gateLatch}, steps{steps} {}

    ~Sequence() = default;

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
    std::vector<std::unique_ptr<Step>> &steps;
  };
} // namespace curve_sequencer
} // namespace dhe
