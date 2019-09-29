#pragma once

#include "modules/curve-sequencer/Step.h"

#include <modules/components/Latch.h>
#include <vector>

namespace dhe {
namespace curve_sequencer {
  /**
   * Controls the sequence in a CurveSequencer module.
   * @tparam M the type of module that owns this sequence
   * @tparam S the type of step container for the sequence
   */
  template <typename M, typename S = std::vector<Step<M>>, typename L = Latch> class Sequence {
  public:
    Sequence(M &module, S &steps, L &runLatch, L &gateLatch) :
        module{module}, steps{steps}, runLatch{runLatch}, gateLatch{gateLatch} {}

    void process(float sampleTime) {
      runLatch.step(module.isRunning());
      if (!runLatch.high()) {
        return;
      }
      gateLatch.step(module.gate());
      if (gateLatch.rise()) {
        steps[0].process(sampleTime);
      }
    }

  private:
    M &module;
    S &steps;
    L &runLatch;
    L &gateLatch;
  };
} // namespace curve_sequencer
} // namespace dhe
