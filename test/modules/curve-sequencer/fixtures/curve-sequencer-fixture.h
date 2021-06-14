#pragma once
#include "components/latch.h"
#include "modules/curve-sequencer/curve-sequencer.h"

#include "helpers/latches.h"
#include "step-event-enums.h"

#include <dheunit/test.h>

#include <array>
#include <functional>

namespace test {
namespace curve_sequencer {
using dhe::Latch;
using dhe::curve_sequencer::StepEvent;
using dhe::unit::Tester;
static auto constexpr step_count = 8;
static auto constexpr invalid_step = 999;
static auto constexpr no_step = -1;

struct Controls {
  bool is_gated_{};           // NOLINT
  bool is_reset_{};           // NOLINT
  bool is_running_{};         // NOLINT
  float input_ = -199.F;      // NOLINT
  float got_output_ = -299.F; // NOLINT

  Controls(Tester &t) : t_{t} {}

  auto is_gated() const -> bool { return is_gated_; }
  auto is_reset() const -> bool { return is_reset_; }
  auto is_running() const -> bool { return is_running_; }
  auto input() const -> float { return input_; }

  void output(float voltage) { got_output_ = voltage; }

  void assert_output(float want) {
    if (got_output_ != want) {
      t_.errorf("Got output {}, want {}", got_output_, want);
    }
  }

private:
  Tester &t_; // NOLINT
};

struct StepController {
  int first_ = invalid_step;                    // NOLINT
  StepEvent step_event_ = StepEvent::Completed; // NOLINT

  int got_step_ = invalid_step; // NOLINT
  Latch got_gate_{};            // NOLINT
  float got_sample_time_{};     // NOLINT

  StepController(Tester &t) : t_{t} {}

  void enter(int step) { got_step_ = step; }

  auto execute(Latch const &gate, float sample_time) -> StepEvent {
    if (got_step_ < 0 || got_step_ >= step_count) {
      t_.fatalf("Called step_controller.execute() with invalid step {} active",
                got_step_);
    }
    got_gate_ = gate;
    got_sample_time_ = sample_time;
    return step_event_;
  }

  void exit() { got_step_ = no_step; }

private:
  Tester &t_; // NOLINT
};

struct StepSelector {
  int first_ = invalid_step;               // NOLINT
  std::array<int, step_count> successors_; // NOLINT

  StepSelector(Tester &t) : t_{t} {
    for (auto &successor : successors_) {
      successor = invalid_step;
    }
  }

  auto first() const -> int { return first_; }

  auto successor(int current) const -> int {
    if (current < 0 || current >= step_count) {
      t_.errorf("step_selector.successor() called with invalid current step {}",
                current);
    }
    return successors_[current];
  }

private:
  Tester &t_; // NOLINT
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
