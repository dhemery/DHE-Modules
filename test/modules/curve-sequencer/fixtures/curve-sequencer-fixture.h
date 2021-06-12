#pragma once
#include "components/latch.h"
#include "modules/curve-sequencer/curve-sequencer.h"

#include "helpers/latches.h"
#include "step-event-enums.h"

#include <dheunit/test.h>

#include <functional>

namespace test {
namespace curve_sequencer {
using dhe::Latch;
using dhe::curve_sequencer::StepEvent;
using dhe::unit::Tester;

struct Controls {
  Controls(Tester &t) : t_{t} {}
  Tester &t_;         // NOLINT
  bool is_gated_{};   // NOLINT
  bool is_looping_{}; // NOLINT
  bool is_reset_{};   // NOLINT
  bool is_running_{}; // NOLINT
  float output_{};    // NOLINT
  float input_{};     // NOLINT

  bool allow_output_{};  // NOLINT
  bool called_output_{}; // NOLINT
  float got_output_{};   // NOLINT

  auto is_gated() const -> bool { return is_gated_; }
  auto is_looping() const -> bool { return is_looping_; }
  auto is_reset() const -> bool { return is_reset_; }
  auto is_running() const -> bool { return is_running_; }
  auto input() const -> float { return input_; }
  auto output() const -> float { return output_; }

  void output(float v) {
    called_output_ = true;
    if (!allow_output_) {
      t_.errorf("Called output({})", v);
      return;
    }
    got_output_ = v;
  }

  void check_output(float want) {
    if (!called_output_) {
      t_.error("Did not call controls.output(v)");
    }
    if (got_output_ != want) {
      t_.errorf("Got controls output {}, want {}", got_output_, want);
    }
  }
};

struct StepController {
  StepController(Tester &t) : t_{t} {}
  Tester &t_;            // NOLINT
  StepEvent step_event_; // NOLINT

  bool allow_enter_;  // NOLINT
  int got_step_;      // NOLINT
  bool called_enter_; // NOLINT

  bool allow_execute_;    // NOLINT
  bool called_execute_;   // NOLINT
  Latch got_gate_;        // NOLINT
  float got_sample_time_; // NOLINT

  bool allow_exit_;  // NOLINT
  bool called_exit_; // NOLINT

  void enter(int step) {
    called_enter_ = true;
    if (!allow_enter_) {
      t_.errorf("Called enter({})", step);
    }
    got_step_ = step;
  }

  auto execute(Latch const &gate, float sample_time) -> StepEvent {
    called_execute_ = true;
    if (allow_execute_) {
      t_.errorf("Called execute({}, {})", gate, sample_time);
    }
    got_gate_ = gate;
    got_sample_time_ = sample_time;
    return step_event_;
  }

  void exit() {
    called_exit_ = true;
    if (!allow_exit_) {
      t_.errorf("Called exit()");
    }
    called_exit_ = true;
  }

  void check_executed(Latch const &want_gate, float want_sample_time) const {
    if (!called_execute_) {
      t_.error("Did not call step_controller.execute()");
      return;
    }
    if (got_gate_ != want_gate) {
      t_.errorf("step_controller.execute() got gate {}, want {}", got_gate_,
                want_gate);
    }
    if (got_sample_time_ != want_sample_time) {
      t_.errorf("step_controller.execute() got sample time {}, want {}",
                got_sample_time_, want_sample_time);
    }
  }
};

struct StepSelector {
  StepSelector(Tester &t) : t_{t} {}
  Tester &t_; // NOLINT

  bool allow_first_{};  // NOLINT
  bool called_first_{}; // NOLINT
  int first_;           // NOLINT

  bool allow_successor_{};  // NOLINT
  bool called_successor_{}; // NOLINT
  int got_current_;         // NOLINT
  bool got_is_looping_;     // NOLINT
  int successor_;           // NOLINT

  auto first() -> int {
    called_first_ = true;
    if (!allow_first_) {
      t_.error("Called step_selector.first()");
    }
    return first_;
  }
  auto successor(int current, bool is_looping) -> int {
    called_successor_ = true;
    if (!allow_successor_) {
      t_.errorf("Called step_selector.successor({}, {})", current, is_looping);
    }
    got_current_ = current;
    got_is_looping_ = is_looping;
    return successor_;
  }
};

struct Context {
  Context(Controls &controls, StepSelector &step_selector,
          StepController &step_controller)
      : controls_{controls}, step_selector_{step_selector},
        step_controller_{step_controller} {}
  Controls &controls_;              // NOLINT
  StepSelector &step_selector_;     // NOLINT
  StepController &step_controller_; // NOLINT
};

using CurveSequencer =
    dhe::curve_sequencer::CurveSequencer<Controls, StepSelector,
                                         StepController>;

using SetState = std::function<void(Context &, CurveSequencer &)>;
using SetConditions = std::function<void(Context &)>;
using Check = std::function<void(Context &)>;
} // namespace curve_sequencer
} // namespace test
