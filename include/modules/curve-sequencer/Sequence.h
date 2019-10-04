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
  class Sequence {
  public:
    Sequence(SequenceControls &controls, Latch &runLatch, Latch &gateLatch, std::vector<std::unique_ptr<Step>> &steps);
    ~Sequence() = default;
    void process(float sampleTime);

  private:
    void start(float sampleTime);

    SequenceControls &controls;
    Latch &runLatch;
    Latch &gateLatch;
    std::vector<std::unique_ptr<Step>> &steps;
  };
} // namespace curve_sequencer
} // namespace dhe
