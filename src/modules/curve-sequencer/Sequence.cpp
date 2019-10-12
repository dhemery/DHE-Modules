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

    if (activeStep == nullptr && gateLatch.isRise()) {
      activeStep = nextAvailableStep();
      if (activeStep == nullptr) { // No available step
        return;
      }
    }

    if (activeStep != nullptr) {
      auto const state = activeStep->process(gateLatch, sampleTime);
      if (state == Step::State::Inactive) {
        activeStep = nextAvailableStep();
        if (activeStep == nullptr) { // No available step
          return;
        }
      }
      return;
    }
  }

  auto Sequence::nextAvailableStep() const -> Step * {
    auto const selectionStart = controls.selectionStart();
    auto const selectionLength = controls.selectionLength();

    auto const searchStart = activeStep == nullptr ? selectionStart : activeStep->index() + 1;
    auto const searchEnd = selectionStart + selectionLength - 1;

    for (int i = searchStart; i <= searchEnd; i++) {
      auto const index = i & stepIndexMask;
      auto const step = steps[index].get();
      if (step->isAvailable()) {
        return step;
      }
    }
    return nullptr;
  }
} // namespace curve_sequencer
} // namespace dhe