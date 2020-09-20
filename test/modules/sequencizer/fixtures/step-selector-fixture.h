#pragma once

#include "modules/sequencizer/step-selector.h"
#include <array>
#include <dheunit/test.h>

namespace test {
namespace sequencizer {
using dhe::unit::Tester;
using dhe::unit::TestFunc;

auto constexpr step_count = 8;

struct Module {
  auto selection_start() const -> int { return start_; }
  auto selection_length() const -> int { return length_; }
  auto is_enabled(int step) const -> bool { return enabled_[step]; }
  int start_{};                            // NOLINT
  int length_{};                           // NOLINT
  std::array<bool, step_count> enabled_{}; // NOLINT
};

using StepSelector = dhe::sequencizer::StepSelector<Module>;

template <typename Run> static inline auto test(Run run) -> TestFunc {
  return [run](Tester &t) {
    Module module{};
    StepSelector selector{module, step_count};
    run(t, module, selector);
  };
}
} // namespace sequencizer
} // namespace test
