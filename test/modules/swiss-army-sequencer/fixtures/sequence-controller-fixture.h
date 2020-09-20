#pragma once

#include "modules/cv-sequencer/sequence-controller.h"
#include "modules/cv-sequencer/status.h"
#include "modules/cv-sequencer/step-controller.h"
#include <array>
#include <dheunit/test.h>

namespace test {
namespace cv_sequencer {
using dhe::Latch;
using dhe::cv_sequencer::StepStatus;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

auto constexpr step_count = 8;

struct Module {
  auto gate() const -> bool { return gate_; }
  auto is_looping() const -> bool { return looping_; }
  auto is_reset() const -> bool { return reset_; }
  auto is_running() const -> bool { return running_; }
  auto input() const -> float { return input_; }
  void output(float v) { output_ = v; }
  float input_{};  // NOLINT
  bool gate_{};    // NOLINT
  bool looping_{}; // NOLINT
  float output_{}; // NOLINT
  bool reset_{};   // NOLINT
  bool running_{}; // NOLINT
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
  auto successor(int step, bool looping) -> int {
    called_ = true;
    step_ = step;
    looping_ = looping;
    return successor_;
  }
  bool called_{};   // NOLINT
  int first_{};     // NOLINT
  bool looping_{};  // NOLINT
  int step_{};      // NOLINT
  int successor_{}; // NOLINT
};

using SequenceController =
    dhe::cv_sequencer::SequenceController<Module, StepSelector,
                                                  StepController>;

template <typename Prepare, typename Run>
static inline auto test(Prepare prepare, Run run) -> TestFunc {
  return [prepare, run](Tester &t) {
    Module module{};
    StepController step_controller{};
    StepSelector step_selector{};
    SequenceController sequence_controller{module, step_selector,
                                           step_controller};
    prepare(module, step_selector, step_controller, sequence_controller);
    run(t, module, step_selector, step_controller, sequence_controller);
  };
}
} // namespace cv_sequencer
} // namespace test