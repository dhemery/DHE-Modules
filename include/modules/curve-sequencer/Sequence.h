#pragma once

#include "modules/curve-sequencer/Step.h"

#include <vector>

namespace dhe {
namespace curve_sequencer {
  /**
   * Controls the sequence in a CurveSequencer module.
   * @tparam M the type of module that owns this sequence
   * @tparam S the type of step container for the sequence
   */
  template <typename M, typename S = std::vector<Step<M>>> class Sequence {
  public:
    Sequence(M &module, S &steps) : module{module}, steps{steps} {}

    void process(float /*sampleTime*/) {
      if (!module.isRunning()) {
        return;
      }
      if (module.gate()) {
        steps[0].start();
      }
    }

  private:
    M &module;
    S &steps;
  };
} // namespace curve_sequencer
} // namespace dhe
