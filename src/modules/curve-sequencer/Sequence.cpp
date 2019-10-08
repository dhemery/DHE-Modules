#include "modules/curve-sequencer/Sequence.h"

#include "modules/components/Latch.h"
#include "modules/curve-sequencer/SequenceControls.h"
#include "modules/curve-sequencer/Step.h"

#include <memory>
#include <vector>

namespace dhe {
namespace curve_sequencer {

  void Sequence::process(float sampleTime) {
    runLatch.clock(controls.isRunning());
    gateLatch.clock(controls.gate());

    if (runLatch.isLow()) {
      return;
    }
    if (gateLatch.isRise()) {
      start(sampleTime);
    }
  }

  void Sequence::start(float sampleTime) {
    auto const first = controls.selectionStart();
    auto const length = controls.selectionLength();
    auto const mask = steps.size() - 1;

    for (int i = 0; i < length; i++) {
      auto const index = (first + i) & mask;
      auto &step = steps[index];
      if (step->isAvailable()) {
        step->process(gateLatch, sampleTime);
        return;
      }
    }
  }
} // namespace curve_sequencer
} // namespace dhe
