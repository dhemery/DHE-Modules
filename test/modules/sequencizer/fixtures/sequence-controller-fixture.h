#pragma once

#include "modules/sequencizer/sequence-controller.h"
#include "modules/sequencizer/status.h"
#include "modules/sequencizer/step-controller.h"

#include <dheunit/test.h>

#include <array>
#include <functional>

namespace test {
namespace sequencizer {
using dhe::Latch;
using dhe::sequencizer::StepStatus;
using dhe::unit::Tester;

using TestFunc = std::function<void(Tester &)>;

auto constexpr step_count = 8;

struct Signals {
  auto gate() const -> bool { return gate_; }
  auto is_looping() const -> bool { return looping_; }
  auto is_reset() const -> bool { return reset_; }
  auto is_running() const -> bool { return running_; }
  auto input() const -> float { return in_a_; }
  void output(float v) { output_ = v; }
  void show_sequence_event(bool /**/) {}
  void show_step_event(bool /**/) {}
  void show_step_status(int step, StepStatus status) {
    step_ = step, status_ = status;
  }
  bool gate_{};         // NOLINT
  float in_a_{};        // NOLINT
  bool looping_{};      // NOLINT
  float output_{};      // NOLINT
  bool reset_{};        // NOLINT
  bool running_{};      // NOLINT
  StepStatus status_{}; // NOLINT
  int step_{};          // NOLINT
};

struct StepController {
  void enter(int step) {
    called_ = true;
    entered_ = true;
    entered_step_ = step;
  }
  auto execute(Latch &latch, float sample_time) -> StepStatus {
    called_ = true;
    executed_ = true;
    executed_latch_ = latch;
    executed_sample_time_ = sample_time;
    return status_;
  }
  void exit() { exited_ = true; }
  bool called_{};                // NOLINT
  bool entered_{};               // NOLINT
  StepStatus status_{};          // NOLINT
  bool executed_{};              // NOLINT
  bool exited_{};                // NOLINT
  Latch executed_latch_{};       // NOLINT
  float executed_sample_time_{}; // NOLINT
  int entered_step_{};           // NOLINT
};

struct StepSelector {
  auto first() -> int {
    called_ = true;
    return first_;
  }
  auto successor(int step) -> int {
    called_ = true;
    step_ = step;
    return successor_;
  }
  bool called_{};   // NOLINT
  int first_{};     // NOLINT
  int step_{};      // NOLINT
  int successor_{}; // NOLINT
};

using SequenceController =
    dhe::sequencizer::SequenceController<Signals, StepSelector, StepController>;

template <typename Prepare, typename Run>
static inline auto test(Prepare prepare, Run run) -> TestFunc {
  return [prepare, run](Tester &t) {
    Signals signals{};
    StepController step_controller{};
    StepSelector step_selector{};
    SequenceController sequence_controller{signals, step_selector,
                                           step_controller};
    prepare(signals, step_selector, step_controller, sequence_controller);
    run(t, signals, step_selector, step_controller, sequence_controller);
  };
}
} // namespace sequencizer
} // namespace test
