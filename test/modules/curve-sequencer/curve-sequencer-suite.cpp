#include "modules/curve-sequencer/curve-sequencer.h"

#include "fixtures/curve-sequencer-fixture.h"

#include <dheunit/test.h>

#include <functional>
#include <string>
#include <vector>

namespace test {
namespace curve_sequencer {
using dhe::curve_sequencer::StepEvent;
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::high_latch;
// TODO: Test that cs starts idle

struct StateTest {
  std::string name_;    // NOLINT
  SetConditions setup_; // NOLINT
  void run(Tester &t, Context &context, CurveSequencer &curve_sequencer) const;
};

struct StateSuite {
  std::string name_;             // NOLINT
  SetState enter_state_;         // NOLINT
  std::vector<StateTest> tests_; // NOLINT
  void run(Tester &t) const;
};

// CurveSequencer starts idle, so this is a no-op.
static inline void become_idle(Context &, CurveSequencer &) {} // NOLINT

static void activate_step(Context &context, CurveSequencer &cs); // NOLINT

// Note: Context throws if a command is called without being explicitly allowed
auto idle_tests = StateSuite{
    .name_ = "Idle",
    .enter_state_ = become_idle,
    .tests_ =
        {
            {
                .name_ = "running: reset high: copy input to output",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = true;
                      context.controls_.is_reset_ = true;
                      context.controls_.input_ = 3.9434F;
                      context.controls_.want_output(3.9434F);
                    },
            },
            {
                .name_ = "running: reset low: does nothing",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = true;
                      context.controls_.is_reset_ = false;
                    },
            },
            {
                .name_ = "running: gate low: does nothing",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = true;
                      context.controls_.is_gated_ = false;
                    },
            },
            {
                .name_ = "running: gate rises: "
                         "executes first step with gate edge cleared",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = true;
                      context.controls_.is_gated_ = true; // Causes rise
                      context.step_selector_.want_first(3);
                      context.step_controller_.want_enter(3);
                      context.step_controller_.want_execute(
                          StepEvent::Generated, high_latch, 0.1F);
                    },
            },
            {
                .name_ = "running: gate rises: does nothing if no first step",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = true;
                      context.controls_.is_gated_ = true;    // Causes rise
                      context.step_selector_.want_first(-1); // No first step
                    },
            },
            {
                .name_ = "paused: reset low: does nothing",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = false;
                      context.controls_.is_reset_ = false;
                    },
            },
            {
                .name_ = "paused: reset rise: does nothing",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = false;
                      context.controls_.is_reset_ = true;
                    },
            },
            {
                .name_ = "paused: gate low: does nothing",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = false;
                      context.controls_.is_gated_ = false;
                    },
            },
            {
                .name_ = "paused: gate rise: does nothing",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = false;
                      context.controls_.is_gated_ = true;
                    },
            },
        },
};

auto active_tests = StateSuite{
    .name_ = "Active",
    .enter_state_ = activate_step,
    .tests_ =
        {
            {
                .name_ = "paused: gate low does nothing",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = false;
                      context.controls_.is_gated_ = false;
                    },
            },
            {
                .name_ = "paused: gate rise does nothing",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = false;
                      context.controls_.is_gated_ = true;
                    },
            },
            {
                .name_ = "paused: reset low does nothing",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = false;
                      context.controls_.is_reset_ = false;
                    },
            },
            {
                .name_ = "paused: "
                         "reset rise exits active step and does nothing else",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = false;
                      context.controls_.is_reset_ = true;
                      context.step_controller_.want_exit();
                    },
            },
            {
                .name_ = "running: "
                         "executes active step with gate state",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = true;
                      context.controls_.is_gated_ = true;
                      // Resets gate edge on first call to execute()
                      context.step_controller_.want_execute(
                          StepEvent::Generated, high_latch, 0.1F);
                      // TODO: Other gate states: rising, falling, low
                      // TODO: Fix magic constant 0.1F
                    },
            },
            {
                .name_ = "running: "
                         "enters successor state if step completes",
                .setup_ =
                    [](Context &context) {
                      auto constexpr current_step = 2;
                      auto constexpr successor_step = 4;
                      context.controls_.is_running_ = true;
                      context.controls_.is_gated_ = true;
                      context.controls_.is_looping_ = true;

                      context.step_controller_.want_execute(
                          StepEvent::Completed, high_latch, 0.1F);
                      context.step_selector_.want_successor(
                          successor_step, current_step,
                          context.controls_.is_looping_);
                      context.step_controller_.want_enter(successor_step);
                    },
            },
            {
                .name_ = "running: step completes: no successor: "
                         "exit current step and does nothing else",
                .setup_ =
                    [](Context &context) {
                      auto constexpr current_step = 2;
                      context.controls_.is_running_ = true;
                      context.controls_.is_gated_ = true;

                      context.step_controller_.want_execute(
                          StepEvent::Completed, high_latch, 0.1F);
                      context.step_selector_.want_successor(
                          -1, current_step, context.controls_.is_looping_);
                    },
            },
            {
                .name_ = "running: reset rises: "
                         "copies input to output, exits current step, "
                         "and does nothing else",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = true;
                      context.controls_.is_reset_ = true;
                      context.controls_.input_ = 5.8340F;

                      context.step_controller_.want_exit();
                      context.controls_.want_output(5.8340F);
                    },
            },
        },
};

struct CurveSequencerSuite : public Suite {
  CurveSequencerSuite() : Suite{"dhe::curve_suquencer::CurveSequencer"} {}
  void run(Tester &t) override {
    idle_tests.run(t);
    active_tests.run(t);
  }
};

void StateTest::run(Tester &t, Context &context,
                    CurveSequencer &curve_sequencer) const {
  t.run(name_, [this, &context, &curve_sequencer](Tester &t) {
    setup_(context);
    curve_sequencer.execute(0.1F);
    context.check_required_calls();
  });
}

void StateSuite::run(Tester &t) const {
  t.run(name_, [this](Tester &t) {
    for (auto const &test : tests_) {
      auto controls = Controls{t};
      auto step_selector = StepSelector{t};
      auto step_controller = StepController{t};
      auto context = Context{controls, step_selector, step_controller};
      auto curve_sequencer =
          CurveSequencer{controls, step_selector, step_controller};

      enter_state_(context, curve_sequencer);

      test.run(t, context, curve_sequencer);
    }
  });
}

// TODO: Randomize the active step, and arrange for the test to know which step
//  is active
void activate_step(Context &context, CurveSequencer &cs) {
  context.controls_.is_running_ = true;
  // Gate rise will trigger CS to enter first step and execute it.
  context.controls_.is_gated_ = true;
  auto constexpr first_step = 2;
  context.step_selector_.want_first(first_step);
  context.step_controller_.want_enter(first_step);
  context.step_controller_.want_execute(StepEvent::Generated, high_latch, 0.F);

  cs.execute(0.F);

  context.reset();
}

static auto _ = CurveSequencerSuite{};
} // namespace curve_sequencer
} // namespace test
