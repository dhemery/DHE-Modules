#pragma once

namespace dhe {
namespace curve_sequencer {
  template <typename Controls> class StepSelector {
  public:
    StepSelector(Controls &controls, int stepCount) : controls{controls}, stepMask{stepCount - 1} {}

    auto first() const -> int {
      auto const selectionStart = controls.selectionStart();
      if (isEnabled(selectionStart)) {
        return selectionStart;
      }
      return successor(selectionStart, selectionStart, controls.selectionLength(), false);
    }

    auto successor(int current, bool isLooping) const -> int {
      return successor(current, controls.selectionStart(), controls.selectionLength(), isLooping);
    }

  private:
    auto isEnabled(int step) const -> bool { return controls.isEnabled(step); }

    auto successor(int current, int selectionStart, int selectionLength, bool isLooping) const -> int {
      auto const selectionEnd = (selectionStart + selectionLength - 1) & stepMask;
      if (current == selectionEnd) {
        return isLooping ? first() : -1;
      }
      for (auto i = current + 1; i < selectionStart + selectionLength; i++) {
        auto const candidate = i & stepMask;
        if (isSelected(candidate, selectionStart, selectionEnd) && isEnabled(candidate)) {
          return candidate;
        }
      }
      return isLooping ? first() : -1;
    };

    auto isSelected(int candidate, int selectionStart, int selectionEnd) const -> bool {
      if (selectionEnd >= selectionStart) {
        return candidate >= selectionStart && candidate <= selectionEnd;
      }
      // else the selection wraps from highest step to lowest, e.g. [5 6 7 0 1 2]
      return candidate >= selectionStart || candidate <= selectionEnd;
    }

    Controls &controls;
    int const stepMask;
  };
} // namespace curve_sequencer
} // namespace dhe
