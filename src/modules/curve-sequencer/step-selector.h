#pragma once

namespace dhe {
namespace curve_sequencer {
template <typename Controls> class StepSelector {
public:
  StepSelector(Controls &controls, int step_count)
      : controls_{controls}, step_mask_{step_count - 1} {}

  auto first() const -> int {
    auto const selection_start = controls_.selection_start();
    if (is_enabled(selection_start)) {
      return selection_start;
    }
    return successor(selection_start, selection_start,
                     controls_.selection_length(), false);
  }

  auto successor(int current, bool is_looping) const -> int {
    return successor(current, controls_.selection_start(),
                     controls_.selection_length(), is_looping);
  }

private:
  auto is_enabled(int step) const -> bool { return controls_.is_enabled(step); }

  auto successor(int current, int selection_start, int selection_length,
                 bool is_looping) const -> int {
    auto const selection_end =
        (selection_start + selection_length - 1) & step_mask_;
    if (current == selection_end) {
      return is_looping ? first() : -1;
    }
    for (auto i = current + 1; i < selection_start + selection_length; i++) {
      auto const candidate = i & step_mask_;
      if (is_selected(candidate, selection_start, selection_end) &&
          is_enabled(candidate)) {
        return candidate;
      }
    }
    return is_looping ? first() : -1;
  };

  auto is_selected(int candidate, int selection_start, int selection_end) const
      -> bool {
    if (selection_end >= selection_start) {
      return candidate >= selection_start && candidate <= selection_end;
    }
    // else the selection wraps from highest step to lowest, e.g. [5 6 7 0 1 2]
    return candidate >= selection_start || candidate <= selection_end;
  }

  Controls &controls_;
  int const step_mask_;
};
} // namespace curve_sequencer
} // namespace dhe
