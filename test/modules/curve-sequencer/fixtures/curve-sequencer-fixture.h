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
static auto constexpr no_step = -1;

// Initial values for input variables. These values are invalid. If a test wants
// the engine to use a variable, it will overwrite the value with a valid one.
// So if the engine uses these original values for anything, then either the
// test or the engine did something wrong.
static auto constexpr original_first = 2222;
static auto constexpr original_successor = 3333;
static auto constexpr original_input = 1.111F;
static auto constexpr original_step_event = static_cast<StepEvent>(5555);

// Initial values for output variables that the engine will overwrite under the
// right conditions. Except for original_gate, neither the tests nor the engine
// will write these values. If a test wants to verify that the engine left a
// variable unchanged, it can assert that the variable has its original value.
static auto constexpr original_step = 1111;
static auto constexpr exited_step = 4444;
static auto constexpr original_output = 2.222F;
static auto constexpr original_gate = falling_latch;

struct Controls {
  bool is_gated_{};                // NOLINT
  bool is_reset_{};                // NOLINT
  bool is_running_{};              // NOLINT
  float input_ = original_input;   // NOLINT
  float output_ = original_output; // NOLINT

  Controls(Tester &t) : t_{t} {}

  auto is_gated() const -> bool { return is_gated_; }
  auto is_reset() const -> bool { return is_reset_; }
  auto is_running() const -> bool { return is_running_; }
  auto input() const -> float { return input_; }

  void output(float voltage) { output_ = voltage; }

  void assert_output(float want) {
    if (output_ != want) {
      t_.errorf("Controls output is {}, want {}", output_, want);
    }
  }

private:
  Tester &t_; // NOLINT
};

struct StepController {
  int first_ = original_first;                 // NOLINT
  StepEvent step_event_ = original_step_event; // NOLINT

  int step_ = original_step;   // NOLINT
  Latch gate_ = original_gate; // NOLINT
  float sample_time_{};        // NOLINT

  StepController(Tester &t) : t_{t} {}

  void enter(int step) { step_ = step; }

  auto execute(Latch const &gate, float sample_time) -> StepEvent {
    if (step_ < 0 || step_ >= step_count) {
      t_.fatalf("Called step_controller.execute() with invalid step {} active",
                step_);
    }
    gate_ = gate;
    sample_time_ = sample_time;
    return step_event_;
  }

  void exit() { step_ = exited_step; }

  void assert_step(int want) {
    if (step_ != want) {
      t_.errorf("StepController step is {}, want {}", step_, want);
    }
  }

  void assert_gate(Latch const &want) {
    if (gate_ != want) {
      t_.errorf("StepController gate is {}, want {}", gate_, want);
    }
  }

private:
  Tester &t_; // NOLINT
};

struct StepSelector {
  int first_ = original_step;              // NOLINT
  std::array<int, step_count> successors_; // NOLINT

  StepSelector(Tester &t) : t_{t} {
    for (auto &successor : successors_) {
      successor = original_successor;
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

struct Module2 {
  Controls controls_;
  StepSelector step_selector_;
  StepController step_controller_;
};

using Condition2 = std::function<void(Module2 &)>;
using Context2 = std::vector<Condition2>;
using Check2 = std::function<void(Tester &, Module2 &)>;

static inline auto is_running(bool b) -> Condition2 {
  return [b](Module2 &module) { module.controls_.is_running_ = b; };
}

static inline auto is_gated(bool b) -> Condition2 {
  return [b](Module2 &module) { module.controls_.is_gated_ = b; };
}

static inline auto first_step(int s) -> Condition2 {
  return [s](Module2 &module) { module.step_selector_.first_ = s; };
}

static inline auto step_event(StepEvent e) -> Condition2 {
  return [e](Module2 &module) { module.step_controller_.step_event_ = e; };
}

static inline auto assert_output(float want) -> Check2 {
  return [want](Tester &t, Module2 &module) {
    float got = module.controls_.output_;
    if (got != want) {
      t.errorf("Got output {}, want {}", got, want);
    }
  };
}

static inline auto assert_step(int want) -> Check2 {
  return [want](Tester &t, Module2 &module) {
    int got = module.step_controller_.step_;
    if (got != want) {
      t.errorf("Got step {}, want {}", got, want);
    }
  };
}

static inline auto activate_step(int step) -> Context2 {
  return {
      is_running(true),
      is_gated(true),
      first_step(step),
      step_event(StepEvent::Generated),
  };
}

} // namespace curve_sequencer
} // namespace test
