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
  Check check_;
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
                    },
                .check_ =
                    [](Context &context) {
                      context.controls_.assert_output(original_input);
                      context.step_controller_.assert_step(original_step);
                    },
            },
            {
                .name_ = "running: reset low: does nothing",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = true;
                      context.controls_.is_reset_ = false;
                    },
                .check_ =
                    [](Context &context) {
                      context.controls_.assert_output(original_output);
                      context.step_controller_.assert_step(original_step);
                    },
            },
            {
                .name_ = "running: gate low: does nothing",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = true;
                      context.controls_.is_gated_ = false;
                    },
                .check_ =
                    [](Context &context) {
                      context.controls_.assert_output(original_output);
                      context.step_controller_.assert_step(original_step);
                    },
            },
            {
                .name_ = "running: gate rises: "
                         "executes first step with gate edge cleared",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = true;
                      context.controls_.is_gated_ = true; // Causes rise
                      context.step_selector_.first_ = 3;
                      context.step_controller_.step_event_ =
                          StepEvent::Generated;
                    },
                .check_ =
                    [](Context &context) {
                      context.step_controller_.assert_step(3);
                      context.step_controller_.assert_gate(high_latch);
                    },
            },
            {
                .name_ = "running: gate rises: does nothing if no first step",
                .setup_ =
                    [](Context &context) {
                      context.controls_.is_running_ = true;
                      context.controls_.is_gated_ = true; // Causes rise
                      context.step_selector_.first_ = no_step;
                    },
                .check_ =
                    [](Context &context) {
                      context.step_controller_.assert_step(original_step);
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
        } // namespace test
    ,
};

struct StateTest2 {
  std::string name_;               // NOLINT
  std::vector<Context2> contexts_; // NOLINT
  std::vector<Check2> checks_;     // NOLINT
  void run(Tester &, Module2 &, CurveSequencer &) const;
};

struct StateSuite2 {
  std::string name_;               // NOLINT
  std::vector<Context2> contexts_; // NOLINT
  std::vector<StateTest2> tests_;  // NOLINT
  void run(Tester &t) const;
};

auto active_tests = StateSuite2{
    .name_ = "NewActive",
    .contexts_ =
        {
            activate_step(2),
        },
    .tests_ =
        {
            {
                .name_ = "does nothing if not running and gate is low",
                .contexts_ =
                    {
                        {is_running(false)},
                        {is_gated(false)},
                    },
                .checks_ =
                    {
                        {assert_output(original_output)},
                        {assert_step(original_step)},
                    },
            },
        },
};

/**
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
                  context.step_controller_.want_execute(StepEvent::Generated,
                                                        high_latch, 0.1F);
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

                  context.step_controller_.want_execute(StepEvent::Completed,
                                                        high_latch, 0.1F);
                  context.step_selector_.want_successor(successor_step,
                                                        current_step);
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

                  context.step_controller_.want_execute(StepEvent::Completed,
                                                        high_latch, 0.1F);
                  context.step_selector_.want_successor(-1, current_step);
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
*/

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

void StateTest2::run(Tester &t, Module2 &m, CurveSequencer &cs) const {
  t.run(name_, [this, &m, &cs](Tester &t) {
    // Execute cs in each of the test's contexts.
    for (auto const &context : contexts_) {
      for (auto const &condition : context) {
        condition(m);
      }
      cs.execute(0.F);
    }
    // Apply each check to the final state.
    for (auto const &check : checks_) {
      check(t, m);
    }
  });
}

void StateSuite2::run(Tester &t) const {
  t.run(name_, [this](Tester &t) {
    for (auto const &test : tests_) {
      auto controls = Controls{t};
      auto step_selector = StepSelector{t};
      auto step_controller = StepController{t};

      auto module = Module2{controls, step_selector, step_controller};

      auto curve_sequencer =
          CurveSequencer{controls, step_selector, step_controller};

      // To set up the suite's initial state, execute cs in each of the suite's
      // contexts.
      for (auto const &context : contexts_) {
        for (auto const &condition : context) {
          condition(module);
        }
        curve_sequencer.execute(0.F);
      }
      test.run(t, module, curve_sequencer);
    }
  });
}

static auto _ = CurveSequencerSuite{};
} // namespace curve_sequencer
} // namespace test
