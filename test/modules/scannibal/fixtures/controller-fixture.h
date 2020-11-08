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
  void generate(int step, float phase) {
    step_ = step;
    phase_ = phase;
  }

  int step_{-1};      // NOLINT
  float phase_{-1.F}; // NOLINT
};

struct Module {
  void show_position(int step, float phase) {
    step_number_ = step;
    step_phase_ = phase;
  }

  auto duration(int step) const -> float { return duration_[step]; }
  auto length() const -> int { return length_; }
  auto phase() const -> float { return phase_; }

  std::array<float, step_count> duration_{}; // NOLINT
  int length_{step_count};                   // NOLINT
  float phase_{0.F};                         // NOLINT
  int step_number_{-1};                      // NOLINT
  float step_phase_{-1.F};                   // NOLINT
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
