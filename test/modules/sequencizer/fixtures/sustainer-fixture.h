#pragma once

#include "modules/sequencizer/advancement.h"
#include <dheunit/test.h>
#include <functional>
namespace test {
namespace sequencizer {
using dhe::sequencizer::SustainMode;
using dhe::sequencizer::TriggerMode;
using dhe::unit::Tester;
using TestFunc = std::function<void(Tester &)>;

auto constexpr step_count = 8;

struct Module {
  auto completion_mode(int step) const -> SustainMode {
    return completion_mode_[step];
  }
  auto trigger_mode(int step) const -> TriggerMode {
    return trigger_mode_[step];
  }

  std::array<SustainMode, step_count> completion_mode_{}; // NOLINT
  std::array<TriggerMode, step_count> trigger_mode_{};    // NOLINT
};

using Sustainer = dhe::sequencizer::Sustainer<Module>;

template <typename Run> static inline auto test(Run const &run) -> TestFunc {
  return [run](Tester &t) {
    Module module{};
    Sustainer sustainer{module};
    run(t, module, sustainer);
  };
}
} // namespace sequencizer
} // namespace test
