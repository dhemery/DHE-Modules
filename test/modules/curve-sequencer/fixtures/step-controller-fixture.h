#pragma once

#include "modules/curve-sequencer/step-controller.h"
#include <array>
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {
using dhe::Latch;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

auto constexpr step_count = 8;

struct Generator {
  void start(int step) { started_step_ = step; }
  void stop() { stopped_ = true; }
  auto generate(float sample_time) -> bool {
    sample_time_ = sample_time;
    return generate_result_[started_step_];
  }

  int started_step_ = -3;                          // NOLINT
  float sample_time_ = -9.F;                       // NOLINT
  bool stopped_ = false;                           // NOLINT
  std::array<bool, step_count> generate_result_{}; // NOLINT
};

struct Interrupter {
  auto is_interrupted(int step, Latch const & /*gate*/) -> bool {
    return is_interrupted_[step];
  }
  std::array<bool, step_count> is_interrupted_{}; // NOLINT
};

struct Sustainer {
  auto is_done(int step, Latch const & /*gate*/) -> bool {
    return is_done_[step];
  }
  std::array<bool, step_count> is_done_{}; // NOLINT
};

using StepController =
    dhe::curve_sequencer::StepController<Interrupter, Generator, Sustainer>;

template <typename T> static inline auto test(T controller_test) -> TestFunc {
  return [controller_test](Tester &t) {
    Interrupter interrupter{};
    Generator generator{};
    Sustainer sustainer{};
    StepController controller{interrupter, generator, sustainer};
    controller_test(t, interrupter, generator, sustainer, controller);
  };
}
} // namespace curve_sequencer
} // namespace test
