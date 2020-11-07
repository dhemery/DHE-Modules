#pragma once

#include "modules/scannibal/engine.h"

#include <array>
#include <dheunit/test.h>

namespace test {
namespace scannibal {
using dhe::unit::Tester;
using dhe::unit::TestFunc;

auto constexpr step_count = 8;

struct Anchor {
  void enter(int step) { entered_[step] = true; }
  auto voltage() const -> float { return voltage_; }

  std::array<bool, step_count> entered_{}; // NOLINT
  float voltage_{};                        // NOLINT
};

struct Module {
  auto input() const -> float { return in_a_; }
  void output(float v) { output_ = v; }
  void show_status(int step, float step_phase) {
    step_ = step;
    step_phase_ = step_phase;
  }
  bool gate_{};             // NOLINT
  float in_a_{};            // NOLINT
  float output_{};          // NOLINT
  float step_phase_{999.F}; // NOLINT
  int step_{-39};           // NOLINT
};

using Engine = dhe::scannibal::Engine<Module, Anchor>;

template <typename Run> static inline auto test(Run run) -> TestFunc {
  return [run](Tester &t) {
    Module module{};
    Anchor start_anchor{};
    Anchor end_anchor{};
    Engine engine{module, start_anchor, end_anchor};
    run(t, module, start_anchor, end_anchor, engine);
  };
}
} // namespace scannibal
} // namespace test
