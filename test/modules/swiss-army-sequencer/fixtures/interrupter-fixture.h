#pragma once

#include "modules/swiss-army-sequencer/advancement.h"
#include <dheunit/test.h>

namespace test {
namespace swiss_army_sequencer {
using dhe::swiss_army_sequencer::InterruptMode;
using dhe::swiss_army_sequencer::TriggerMode;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

auto constexpr step_count = 8;

struct Module {
  auto interrupt_mode(int step) const -> InterruptMode {
    return interrupt_mode_[step];
  }
  auto trigger_mode(int step) const -> TriggerMode {
    return trigger_mode_[step];
  }

  std::array<InterruptMode, step_count> interrupt_mode_{}; // NOLINT
  std::array<TriggerMode, step_count> trigger_mode_{};     // NOLINT
};

using Interrupter = dhe::swiss_army_sequencer::Interrupter<Module>;

template <typename Run> static inline auto test(Run const &run) -> TestFunc {
  return [run](Tester &t) {
    Module module{};
    Interrupter interrupter{module};
    run(t, module, interrupter);
  };
}
} // namespace swiss_army_sequencer
} // namespace test
