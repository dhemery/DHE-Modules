#pragma once

#include "modules/curve-sequencer/step-selector.h"
#include <array>
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {
using dhe::unit::Tester;
using dhe::unit::TestFunc;

auto constexpr step_count = 8;

struct Controls {
  auto selection_start() const -> int { return start_; }
  auto selection_length() const -> int { return length_; }
  auto is_enabled(int step) const -> bool { return enabled_[step]; }
  int start_{};                            // NOLINT
  int length_{};                           // NOLINT
  std::array<bool, step_count> enabled_{}; // NOLINT
};

using StepSelector = dhe::curve_sequencer::StepSelector<Controls>;

template <typename Run> static inline auto test(Run run) -> TestFunc {
  return [run](Tester &t) {
    Controls controls{};
    StepSelector selector{controls, step_count};
    run(t, controls, selector);
  };
}
} // namespace curve_sequencer
} // namespace test
