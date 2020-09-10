#pragma once

#include "modules/cv-sequencer/advancement.h"
#include <dheunit/test.h>

namespace test {
namespace cv_sequencer {
using dhe::cv_sequencer::CompletionMode;
using dhe::cv_sequencer::TriggerMode;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

auto constexpr step_count = 8;

struct Module {
  auto completion_mode(int step) const -> CompletionMode {
    return completion_mode_[step];
  }
  auto trigger_mode(int step) const -> TriggerMode {
    return trigger_mode_[step];
  }

  std::array<CompletionMode, step_count> completion_mode_{}; // NOLINT
  std::array<TriggerMode, step_count> trigger_mode_{};       // NOLINT
};

using Sustainer = dhe::cv_sequencer::Sustainer<Module>;

template <typename Run> static inline auto test(Run const &run) -> TestFunc {
  return [run](Tester &t) {
    Module module{};
    Sustainer sustainer{module};
    run(t, module, sustainer);
  };
}
} // namespace cv_sequencer
} // namespace test
