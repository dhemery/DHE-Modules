#pragma once

#include "modules/sequencizer/advancement.h"

#include <dheunit/test.h>

#include <functional>

namespace test {
namespace sequencizer {
using dhe::sequencizer::InterruptMode;
using dhe::sequencizer::TriggerMode;
using dhe::unit::Tester;

using TestFunc = std::function<void(Tester &)>;

auto constexpr step_count = 8;

struct Signals {
  auto interrupt_mode(int step) const -> InterruptMode {
    return interrupt_mode_[step];
  }
  auto trigger_mode(int step) const -> TriggerMode {
    return trigger_mode_[step];
  }

  std::array<InterruptMode, step_count> interrupt_mode_{}; // NOLINT
  std::array<TriggerMode, step_count> trigger_mode_{};     // NOLINT
};

using Interrupter = dhe::sequencizer::Interrupter<Signals>;

template <typename Run> static inline auto test(Run const &run) -> TestFunc {
  return [run](Tester &t) {
    Signals module{};
    Interrupter interrupter{module};
    run(t, module, interrupter);
  };
}
} // namespace sequencizer
} // namespace test
