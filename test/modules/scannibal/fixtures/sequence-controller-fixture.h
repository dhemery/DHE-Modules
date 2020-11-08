#pragma once

#include "modules/scannibal/controller.h"

#include <array>
#include <dheunit/test.h>

namespace test {
namespace scannibal {
using dhe::unit::Tester;
using dhe::unit::TestFunc;

auto constexpr step_count = 8;

struct Generator {
  void execute(int step, float phase) {
    executed_[step] = true;
    phase_[step] = phase;
  }

  std::array<bool, step_count> executed_{}; // NOLINT
  std::array<bool, step_count> phase_{};    // NOLINT
};

struct Module {
  void show_status(int step, float phase) {
    step_number_ = step;
    step_phase_ = phase;
  }

  float steps_{step_count}; // NOLINT
  float phase_{};           // NOLINT
  int step_number_{-1};     // NOLINT
  float step_phase_{-1.F};  // NOLINT
};

using Controller = dhe::scannibal::Controller<Module, Generator>;

template <typename Run> static inline auto test(Run run) -> TestFunc {
  return [run](Tester &t) {
    Module module{};
    Generator generator{};
    Controller controller{module, generator};
    run(t, module, generator, controller);
  };
}
} // namespace scannibal
} // namespace test
