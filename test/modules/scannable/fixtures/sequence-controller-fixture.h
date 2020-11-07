#pragma once

#include "modules/scannable/sequence-controller.h"
#include <array>
#include <dheunit/test.h>

namespace test {
namespace scannable {
using dhe::Latch;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

auto constexpr step_count = 8;

struct Module {
  auto input() const -> float { return in_a_; }
  void output(float v) { output_ = v; }
  void show_step_phase(int step, float step_phase) {
    step_ = step;
    step_phase_ = step_phase;
  }
  bool gate_{};        // NOLINT
  float in_a_{};       // NOLINT
  float output_{};     // NOLINT
  float step_phase_{}; // NOLINT
  int step_{};         // NOLINT
};

struct Generator {
  void enter(int step) {
    called_ = true;
    entered_ = true;
    entered_step_ = step;
  }

  void execute(float phase) {
    called_ = true;
    executed_ = true;
    phase_ = phase;
  }
  void exit() { exited_ = true; }
  bool called_{};      // NOLINT
  bool entered_{};     // NOLINT
  bool executed_{};    // NOLINT
  bool exited_{};      // NOLINT
  float phase_{};      // NOLINT
  int entered_step_{}; // NOLINT
};

using SequenceController =
    dhe::scannable::SequenceController<Module, Generator>;

template <typename Run> static inline auto test(Run run) -> TestFunc {
  return [run](Tester &t) {
    Module module{};
    Generator generator{};
    SequenceController sequence_controller{module, generator};
    run(t, module, generator, sequence_controller);
  };
}
} // namespace scannable
} // namespace test
