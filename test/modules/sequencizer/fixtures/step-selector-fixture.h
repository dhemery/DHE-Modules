#pragma once

#include "modules/sequencizer/step-selector.h"

#include "dheunit/test.h"

#include <array>
#include <functional>

namespace test {
namespace sequencizer {
using dhe::unit::Tester;

using TestFunc = std::function<void(Tester &)>;

auto constexpr step_count = 8;

struct Signals {
  auto selection_start() const -> int { return start_; }
  auto selection_length() const -> int { return length_; }
  auto is_enabled(int step) const -> bool { return enabled_[step]; }
  int start_{};                            // NOLINT
  int length_{};                           // NOLINT
  std::array<bool, step_count> enabled_{}; // NOLINT
};

using StepSelector = dhe::sequencizer::StepSelector<Signals>;

template <typename Run> static inline auto test(Run run) -> TestFunc {
  return [run](Tester &t) {
    Signals signals{};
    StepSelector selector{signals, step_count};
    run(t, signals, selector);
  };
}
} // namespace sequencizer
} // namespace test
