#pragma once
#include "components/latch.h"
#include "modules/curve-sequencer/curve-sequencer.h"

#include "helpers/latches.h"
#include "step-event-enums.h"

#include <dheunit/test.h>

#include <array>
#include <functional>
#include <utility>

namespace test {
namespace curve_sequencer {
using dhe::Latch;
using dhe::curve_sequencer::StepEvent;
using dhe::unit::Tester;
static auto constexpr step_count = 8;
static auto constexpr no_step = -1;

static auto constexpr original_step = 1111;
static auto constexpr original_first = 2222;
static auto constexpr original_successor = 3333;
static auto constexpr out_of_range_step = 4444;
static auto constexpr exited_step = 5555;

static auto constexpr original_input = 6666.F;
static auto constexpr original_output = 7777.F;

static auto constexpr original_step_event = static_cast<StepEvent>(8888);
static auto constexpr out_of_range_step_event = static_cast<StepEvent>(9999);
static auto constexpr original_gate = falling_latch;

struct Controls {
  auto is_gated() const -> bool { return is_gated_; }
  auto is_reset() const -> bool { return is_reset_; }
  auto is_running() const -> bool { return is_running_; }
  auto input() const -> float { return input_; }

  void output(float f) { output_ = f; }

  // Inputs
  bool is_gated_{};              // NOLINT
  bool is_reset_{};              // NOLINT
  bool is_running_{};            // NOLINT
  float input_ = original_input; // NOLINT

  // Captures
  float output_ = original_output; // NOLINT
};

struct StepController {
  StepController() {
    for (auto &step_event : step_events_) {
      step_event = original_step_event;
    };
  }

  void enter(int step) { step_ = step; }

  auto execute(Latch const &gate, float sample_time) -> StepEvent {
    gate_ = gate;
    sample_time_ = sample_time;
    if (step_ < 0 || step_ >= step_count) {
      return out_of_range_step_event;
    }
    return step_events_[step_];
  }

  void exit() { step_ = exited_step; }

  // Inputs
  std::array<StepEvent, step_count> step_events_{}; // NOLINT

  // Captures
  int step_ = original_step;   // NOLINT
  Latch gate_ = original_gate; // NOLINT
  float sample_time_{};        // NOLINT
};

struct StepSelector {
  StepSelector() {
    for (auto &successor : successors_) {
      successor = original_successor;
    }
  }

  auto first() const -> int { return first_; }

  auto successor(int current) const -> int {
    if (current < 0 || current >= step_count) {
      return out_of_range_step;
    }
    return successors_[current];
  }

  // Inputs
  int first_ = original_first;             // NOLINT
  std::array<int, step_count> successors_; // NOLINT
};

using CurveSequencer =
    dhe::curve_sequencer::CurveSequencer<Controls, StepSelector,
                                         StepController>;

struct Module {
  Controls &controls_;
  StepSelector &step_selector_;
  StepController &step_controller_;
};

using Change = std::function<void(Module &)>;
using Check = std::function<void(Tester &, Module const &)>;

// An event is a set of changes in the module's inputs.
struct Event {
  Event(std::initializer_list<Change> changes) : changes_{changes} {}

  void apply(Module &m) const {
    for (auto const &change : changes_) {
      change(m);
    }
  }

private:
  std::vector<Change> changes_{};
};

static inline auto set_first(int s, StepEvent e) -> Change {
  return [s, e](Module &module) {
    module.step_selector_.first_ = s;
    module.step_controller_.step_events_[s] = e;
  };
}

static inline auto set_gate(bool b) -> Change {
  return [b](Module &module) { module.controls_.is_gated_ = b; };
}

static inline auto set_input(float f) -> Change {
  return [f](Module &module) { module.controls_.input_ = f; };
}

static inline auto set_reset(bool b) -> Change {
  return [b](Module &module) { module.controls_.is_reset_ = b; };
}

static inline auto set_running(bool b) -> Change {
  return [b](Module &module) { module.controls_.is_running_ = b; };
}

static inline auto assert_output(float want) -> Check {
  return [want](Tester &t, Module const &module) {
    auto const got = module.controls_.output_;
    if (got != want) {
      t.errorf("Got output {}, want {}", got, want);
    }
  };
}

static inline auto assert_step(int want) -> Check {
  return [want](Tester &t, Module const &module) {
    auto const got = module.step_controller_.step_;
    if (got != want) {
      t.errorf("Got step {}, want {}", got, want);
    }
  };
}

static inline auto assert_gate(Latch const &want) -> Check {
  return [want](Tester &t, Module const &module) {
    auto const got = module.step_controller_.gate_;
    if (got != want) {
      t.errorf("Got gate latch {}, want {}", got, want);
    }
  };
}

static inline auto assert_exited() -> Check {
  return [](Tester &t, Module const &module) {
    auto const step = module.step_controller_.step_;
    if (step != exited_step) {
      t.errorf("Want exit(), but was still in step {}", step);
    }
  };
}

static inline auto activate_step(int step) -> Event {
  return {
      set_running(true),
      set_gate(true),
      set_first(step, StepEvent::Generated),
  };
}

} // namespace curve_sequencer
} // namespace test
