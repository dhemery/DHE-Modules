#pragma once

namespace dhe {
namespace sequencizer {
template <typename Signals> class StepSelector {
public:
  StepSelector(Signals &signals, int step_count)
      : signals_{signals}, step_mask_{step_count - 1} {}

  auto first() const -> int {
    auto const selection_start = signals_.selection_start();
    if (is_enabled(selection_start)) {
      return selection_start;
    }
    return successor(selection_start, selection_start,
                     signals_.selection_length());
  }

  auto successor(int current) const -> int {
    return successor(current, signals_.selection_start(),
                     signals_.selection_length());
  }

private:
  auto is_enabled(int step) const -> bool { return signals_.is_enabled(step); }

  auto successor(int current, int selection_start, int selection_length) const
      -> int {
    auto const selection_end =
        (selection_start + selection_length - 1) & step_mask_;
    if (current == selection_end ||
        !is_selected(current, selection_start, selection_end)) {
      return -1;
    }
    for (auto i = current + 1; i < selection_start + selection_length; i++) {
      auto const candidate = i & step_mask_;
      if (is_selected(candidate, selection_start, selection_end) &&
          is_enabled(candidate)) {
        return candidate;
      }
    }
    return -1;
  };

  auto is_selected(int candidate, int selection_start, int selection_end) const
      -> bool {
    if (selection_end >= selection_start) {
      return candidate >= selection_start && candidate <= selection_end;
    }
    // else the selection wraps from highest step to lowest, e.g. [5 6 7 0 1 2]
    return candidate >= selection_start || candidate <= selection_end;
  }

  Signals &signals_;
  int const step_mask_;
};
} // namespace sequencizer
} // namespace dhe
