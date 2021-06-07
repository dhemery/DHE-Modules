#pragma once

#include <array>
#include <utility>

namespace test {
namespace curve_sequencer {

template <int N> struct StepSelectorControls {
  std::pair<int, int> selection_{-1, -1}; // NOLINT
  std::array<bool, N> is_enabled_{};      // NOLINT

  auto selection_length() -> int { return selection_.second; }
  auto selection_start() -> int { return selection_.first; }
  auto is_enabled(int s) -> bool { return is_enabled_[s]; }
};
} // namespace curve_sequencer
} // namespace test
