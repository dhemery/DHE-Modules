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
    auto const first = controls.selectionStart();
    auto const length = controls.selectionLength();
    auto const mask = steps.size() - 1;

    for (int i = 0; i < length; i++) {
      auto const index = (first + i) & mask;
      auto &step = steps[index];
      if (step->isAvailable()) {
        return index;
      }
    }
    return -1;
  }

  auto Sequence::indexOfSuccessorStep() const -> int {
    auto const first = activeStepIndex + 1;
    auto const length = controls.selectionLength();
    auto const mask = steps.size() - 1;

    for (int i = 0; i < length; i++) {
      auto const index = (first + i) & mask;
      auto &step = steps[index];
      if (step->isAvailable()) {
        return index;
      }
    }
    return -1;
  }
} // namespace curve_sequencer
} // namespace dhe
