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

  auto is_gated() const -> bool { return is_gated_; }
  auto is_looping() const -> bool { return is_looping_; }
  auto is_reset() const -> bool { return is_reset_; }
  auto is_running() const -> bool { return is_running_; }
  auto input() const -> float { return input_; }
  auto output() const -> float { return output_; }

  bool is_gated_{};   // NOLINT
  bool is_looping_{}; // NOLINT
  bool is_reset_{};   // NOLINT
  bool is_running_{}; // NOLINT
  float input_{};     // NOLINT

  void output(float voltage) {
    called_output_ = true;
    if (!want_output_) {
      t_.errorf("Called output({})", voltage);
      return;
    }
    if (voltage != want_output_voltage_) {
      t_.errorf("controls.output() got voltage {}, want {}", voltage,
                want_output_voltage_);
    }
  }

  void want_output(float voltage) {
    want_output_ = true;
    want_output_voltage_ = voltage;
  }

  void check_required_calls() {
    if (want_output_ && !called_output_) {
      t_.error("Did not call controls.output(v)");
    }
  }

private:
  Tester &t_;      // NOLINT
  float output_{}; // NOLINT

  bool want_output_{};          // NOLINT
  bool called_output_{};        // NOLINT
  float want_output_voltage_{}; // NOLINT
};

struct StepController {
  StepController(Tester &t) : t_{t} {}

  void enter(int step) {
    got_enter_ = true;
    if (!want_enter_) {
      t_.errorf("Called enter({})", step);
      return;
    }
    if (step != want_step_) {
      t_.errorf("step_controller.enter() got step {}, want {}", step,
                want_step_);
    }
  }

  auto execute(Latch const &gate, float sample_time) -> StepEvent {
    got_execute_ = true;
    if (want_execute_) {
      if (gate != want_gate_) {
        t_.errorf("step_controller.execute() got gate {}, want {}", gate,
                  want_gate_);
      }
      if (sample_time != want_sample_time_) {
        t_.errorf("step_controller.execute() got sample time {}, want {}",
                  sample_time, want_sample_time_);
      }
    } else {
      t_.errorf("Called execute({}, {})", gate, sample_time);
    }
    return step_event_;
  }

  void exit() {
    got_exit_ = true;
    if (!want_exit_) {
      t_.errorf("Called exit()");
    }
  }

  void want_enter(int step) {
    want_enter_ = true;
    want_step_ = step;
  }

  void want_execute(StepEvent step_event, Latch gate, float sample_time) {
    want_execute_ = true;
    want_gate_ = gate;
    want_sample_time_ = sample_time;
    step_event_ = step_event;
  }

  void want_exit() { want_exit_ = true; }

  void check_required_calls() {
    if (want_enter_ && !got_enter_) {
      t_.error("Did not call step_controller.enter()");
      return;
    }
    if (want_execute_ && !got_execute_) {
      t_.error("Did not call step_controller.execute()");
      return;
    }
    if (want_exit_ && !got_exit_) {
      t_.error("Did not call step_controller.exit()");
      return;
    }
  }

private:
  Tester &t_; // NOLINT

  bool want_enter_{}; // NOLINT
  bool got_enter_{};  // NOLINT
  int want_step_{};   // NOLINT

  bool want_execute_{};      // NOLINT
  StepEvent step_event_{};   // NOLINT
  bool got_execute_{};       // NOLINT
  Latch want_gate_{};        // NOLINT
  float want_sample_time_{}; // NOLINT

  bool want_exit_{}; // NOLINT
  bool got_exit_{};  // NOLINT
};

struct StepSelector {
  StepSelector(Tester &t) : t_{t} {}

  void want_first(int first) {
    want_first_ = true;
    first_ = first;
  }

  auto first() -> int {
    got_first_ = true;
    if (!want_first_) {
      t_.error("Called step_selector.first()");
    }
    return first_;
  }

  auto successor(int current, bool is_looping) -> int {
    got_successor_ = true;
    if (!want_successor_) {
      t_.errorf("Called step_selector.successor({}, {})", current, is_looping);
    }
    if (current != want_current_) {
      t_.errorf("step_selector.successor() got current {}, want {}", current,
                want_current_);
    }
    if (is_looping != want_is_looping_) {
      t_.errorf("step_selector.successor() got is_looping {}, want {}",
                is_looping, want_is_looping_);
    }
    return successor_;
  }

  void check_required_calls() {
    if (want_first_ && !got_first_) {
      t_.error("Did not call step_selector.first()");
      return;
    }
    if (want_successor_ && !got_successor_) {
      t_.error("Did not call step_selector.successor()");
      return;
    }
  }

private:
  Tester &t_; // NOLINT

  bool want_first_{}; // NOLINT
  bool got_first_{};  // NOLINT
  int first_;         // NOLINT

  bool want_successor_{};  // NOLINT
  bool got_successor_{};   // NOLINT
  int want_current_{};     // NOLINT
  bool want_is_looping_{}; // NOLINT
  int successor_{};        // NOLINT
};

struct Context {
  Context(Controls &controls, StepSelector &step_selector,
          StepController &step_controller)
      : controls_{controls}, step_selector_{step_selector},
        step_controller_{step_controller} {}
  Controls &controls_;              // NOLINT
  StepSelector &step_selector_;     // NOLINT
  StepController &step_controller_; // NOLINT

  void check_required_calls() {
    controls_.check_required_calls();
    step_selector_.check_required_calls();
    step_controller_.check_required_calls();
  }
};

using CurveSequencer =
    dhe::curve_sequencer::CurveSequencer<Controls, StepSelector,
                                         StepController>;

using SetState = std::function<void(Context &, CurveSequencer &)>;
using SetConditions = std::function<void(Context &)>;
using Check = std::function<void(Context &)>;
} // namespace curve_sequencer
} // namespace test
