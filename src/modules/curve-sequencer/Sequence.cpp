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

    if (activeStepIndex < 0 && gateLatch.isRise()) {
      activeStepIndex = indexOfFirstAvailableStep();
      if (activeStepIndex < 0) { // No available step
        return;
      }
    }

    if (activeStepIndex >= 0) {
      auto const state = steps[activeStepIndex]->process(gateLatch, sampleTime);
      if (state == Step::State::Terminated) {
        activeStepIndex = indexOfSuccessorStep();
        if (activeStepIndex < 0) { // No available step
          return;
        }
      }
      return;
    }
  }
  auto Sequence::indexOfFirstAvailableStep() const -> int {
    auto const selectionFirst = controls.selectionStart();
    auto const selectionLength = controls.selectionLength();
    auto const selectionLast = selectionFirst + selectionLength - 1;

    for (int i = selectionFirst; i <= selectionLast; i++) {
      auto const index = i & stepIndexMask;
      auto const step = steps[index].get();
      if (step->isAvailable()) {
        return index;
      }
    }
    return -1;
  }

  auto Sequence::indexOfSuccessorStep() const -> int {
    auto const selectionFirst = controls.selectionStart();
    auto const selectionLength = controls.selectionLength();
    auto const selectionLast = selectionFirst + selectionLength - 1;

    for (int i = activeStepIndex + 1; i <= selectionLast; i++) {
      auto const index = i & stepIndexMask;
      auto const step = steps[index].get();
      if (step->isAvailable()) {
        return index;
      }
    }
    return -1;
  }
} // namespace curve_sequencer
} // namespace dhe
