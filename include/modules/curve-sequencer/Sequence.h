#pragma once

#include "modules/curve-sequencer/Step.h"

namespace dhe {
namespace curve_sequencer {
  template <typename M, typename StepT = Step<M>> class Sequence {
  public:
    explicit Sequence(M &module) : module{module} {}

    void process(float sampleTime) {
      if (!module.isRunning()) {
        return;
      }
      module.step(0).start();
    }

  private:
    M &module;
  };
} // namespace curve_sequencer
} // namespace dhe
