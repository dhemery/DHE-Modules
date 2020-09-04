#pragma once

#include "modules/curve-sequencer/completion-mode.h"
#include "modules/curve-sequencer/sustainer.h"
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {
using dhe::curve_sequencer::CompletionMode;
using dhe::curve_sequencer::TriggerMode;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

auto constexpr step_count = 8;

struct Controls {
  auto completion_mode(int step) const -> CompletionMode {
    return completion_mode_[step];
  }
  auto trigger_mode(int step) const -> TriggerMode {
    return trigger_mode_[step];
  }

  std::array<CompletionMode, step_count> completion_mode_{}; // NOLINT
  std::array<TriggerMode, step_count> trigger_mode_{};       // NOLINT
};

using Sustainer = dhe::curve_sequencer::Sustainer<Controls>;

template <typename T>
static inline auto test(T const &sustainer_test) -> TestFunc {
  return [sustainer_test](Tester &t) {
    Controls controls{};
    Sustainer sustainer{controls};
    sustainer_test(t, controls, sustainer);
  };
}
} // namespace curve_sequencer
} // namespace test
