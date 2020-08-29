#pragma once

#include "modules/curve-sequencer/triggers.h"
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {
using dhe::Latch;
using dhe::curve_sequencer::TriggerMode;
using dhe::unit::Tester;

auto constexpr step_count = 8;
struct Controls {
  auto advance_on_end_of_curve(int step) const -> bool {
    return advance_on_end_of_curve_[step];
  }
  auto trigger_mode(int step) const -> TriggerMode {
    return trigger_mode_[step];
  }

  std::array<bool, step_count> advance_on_end_of_curve_{}; // NOLINT
  std::array<TriggerMode, step_count> trigger_mode_{};     // NOLINT
};

using dhe::unit::TestFunc;

using Sustainer = dhe::curve_sequencer::Sustainer<Controls>;

template <typename T>
static inline auto test(T const &sustainer_test) -> TestFunc {
  return [sustainer_test](Tester &t) {
    Controls controls{};
    Sustainer sustainer{controls};
    sustainer_test(t, controls, sustainer);
  };
}

static auto constexpr rising_gate = Latch{true, true};
static auto constexpr falling_gate = Latch{false, true};
static auto constexpr high_gate = Latch{true, false};
static auto constexpr low_gate = Latch{false, false};
} // namespace curve_sequencer
} // namespace test
