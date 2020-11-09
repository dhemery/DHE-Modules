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
    generated_ = true;
    step_ = step;
    phase_ = phase;
  }

  bool generated_{};  // NOLINT
  int step_{-1};      // NOLINT
  float phase_{-1.F}; // NOLINT
};

template <int N> struct M {
  void exit_step(int step) {
    exited_ = true;
    exited_step_ = step;
  }

  void show_position(int step, float phase) {
    showed_position_ = true;
    step_number_ = step;
    step_phase_ = phase;
  }

  auto duration(int step) const -> float { return duration_[step]; }
  auto length() const -> int { return length_; }
  auto phase() const -> float { return phase_; }

  std::array<float, step_count> duration_{}; // NOLINT
  bool exited_{};                            // NOLINT
  int exited_step_{-1};                      // NOLINT
  int length_{step_count};                   // NOLINT
  float phase_{0.F};                         // NOLINT
  bool showed_position_{};                   // NOLINT
  int step_number_{-1};                      // NOLINT
  float step_phase_{-1.F};                   // NOLINT
};

using Controller = dhe::scannibal::Controller<M, Generator, step_count>;
using Module = M<step_count>;

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
