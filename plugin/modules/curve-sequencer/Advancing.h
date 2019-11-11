#pragma once

#include "SequenceMode.h"
#include "SequencerState.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <vector>

namespace dhe {
namespace curve_sequencer {

  template <int N, typename Controls> class Advancing {
  public:
    Advancing(Controls &controls) : controls{controls} {}

    auto execute(int currentStep) const -> SequencerState {
      auto const selectionStart = controls.selectionStart();
      auto const selectionLength = controls.selectionLength();
      if (!isSelected(currentStep, selectionStart, selectionLength)) {
        return {SequenceMode::Idle, currentStep};
      }

      auto const selectionStop = selectionStart + selectionLength;
      for (int index = currentStep; index < selectionStop; index++) {
        auto const step = index & stepMask;
        if (controls.isEnabled(step)) {
          return {SequenceMode::Generating, step};
        }
      }
      return {SequenceMode::Idle, currentStep};
    };

  private:
    auto isSelected(int candidate, int selectionStart, int selectionLength) const -> bool {
      auto const selectionEnd = (selectionStart + selectionLength - 1) & stepMask;
      if (selectionEnd >= selectionStart) {
        return candidate >= selectionStart && candidate <= selectionEnd;
      }
      // else the selection wraps from highest step to lowest, e.g. [5 6 7 0 1 2]
      return candidate >= selectionStart || candidate <= selectionEnd;
    }

    Controls &controls;
    int const stepMask = N - 1;
  };
}; // namespace curve_sequencer
} // namespace dhe
