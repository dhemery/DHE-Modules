#pragma once

#include "SequenceMode.h"
#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {
  using dhe::Latch;

  template <typename Controls, typename StepController> class StepSelector {
  public:
    StepSelector(Controls &controls, StepController &stepController, int stepCount) :
        controls{controls},
        stepController{stepController},
        stepMask{stepCount - 1} {}

    auto first(Latch const &gate) const -> int {
      auto const selectionStart = controls.selectionStart();
      if (isAvailable(selectionStart, gate)) {
        return selectionStart;
      }
      return successor(selectionStart, gate, selectionStart, controls.selectionLength(), false);
    }

    auto successor(int current, Latch const &gate, bool isLooping) const -> int {
      return successor(current, gate, controls.selectionStart(), controls.selectionLength(), isLooping);
    }

  private:
    bool isAvailable(int step, Latch const &gate) const { return stepController.isAvailable(step, gate); }

    auto successor(int current, Latch const &gate, int selectionStart, int selectionLength, bool isLooping) const
        -> int {
      auto const selectionEnd = (selectionStart + selectionLength - 1) & stepMask;
      if (current == selectionEnd) {
        return isLooping ? first(gate) : -1;
      }
      for (int i = current + 1; i < selectionStart + selectionLength; i++) {
        auto const candidate = i & stepMask;
        if (isSelected(candidate, selectionStart, selectionEnd) && isAvailable(candidate, gate)) {
          return candidate;
        }
      }
      return isLooping ? first(gate) : -1;
    };

    auto isSelected(int candidate, int selectionStart, int selectionEnd) const -> bool {
      if (selectionEnd >= selectionStart) {
        return candidate >= selectionStart && candidate <= selectionEnd;
      }
      // else the selection wraps from highest step to lowest, e.g. [5 6 7 0 1 2]
      return candidate >= selectionStart || candidate <= selectionEnd;
    }

    Controls &controls;
    StepController &stepController;
    int const stepMask;
  };
} // namespace curve_sequencer
} // namespace dhe
