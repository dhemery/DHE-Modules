#pragma once

#include "./status-enums.h"
#include "modules/sequencizer/status.h"
#include "modules/sequencizer/step-controller.h"

#include <dheunit/test.h>

#include <array>
#include <functional>

namespace test {
namespace sequencizer {
using dhe::Latch;
using dhe::sequencizer::GeneratorStatus;
using dhe::unit::Tester;

using TestFunc = std::function<void(Tester &)>;

auto constexpr step_count = 8;

struct Generator {
  void start(int step) { started_step_ = step; }
  void stop() { stopped_ = true; }
  auto generate(float sample_time) -> GeneratorStatus {
    sample_time_ = sample_time;
    return status_[started_step_];
  }

  int started_step_ = -3;                            // NOLINT
  float sample_time_ = -9.F;                         // NOLINT
  bool stopped_ = false;                             // NOLINT
  std::array<GeneratorStatus, step_count> status_{}; // NOLINT
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
    dhe::sequencizer::StepController<Interrupter, Generator, Sustainer>;

template <typename Run> static inline auto test(Run run) -> TestFunc {
  return [run](Tester &t) {
    Interrupter interrupter{};
    Generator generator{};
    Sustainer sustainer{};
    StepController controller{interrupter, generator, sustainer};
    run(t, interrupter, generator, sustainer, controller);
  };
}
} // namespace sequencizer
} // namespace test
