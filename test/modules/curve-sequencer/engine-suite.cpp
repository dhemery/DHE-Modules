#include "modules/curve-sequencer/engine.h"

#include "./fixtures/curve-sequencer-fixture.h"

#include "dheunit/test.h"

#include <functional>
#include <string>
#include <vector>

namespace test {
namespace curve_sequencer {
using dhe::curve_sequencer::StepEvent;
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::high_latch;

struct StateTest {
  std::string name_;          // NOLINT
  std::vector<Event> events_; // NOLINT
  std::vector<Check> checks_; // NOLINT
  void run(Tester &t, Module &m, Engine &engine) const;
};

struct StateSuite {
  std::string name_;             // NOLINT
  std::vector<Event> events_;    // NOLINT
  std::vector<StateTest> tests_; // NOLINT
  void run(Tester &t) const;
};

auto newly_constructed_tests = StateSuite{
    .name_ = "Newly constructed",
    .events_ = {},
    .tests_ =
        {
            {
                .name_ = "did nothing",
                .events_ = {},
                .checks_ =
                    {
                        assert_step(original_step),
                        assert_gate(original_gate),
                        assert_output(original_output),
                    },
            },
        },
};

// IDLE: No step in progress
auto idle_tests = StateSuite{
    .name_ = "Idle",
    .events_ = {}, // Constructor leaves it idle
    .tests_ =
        {
            {
                .name_ = "on GATE rise: "
                         "executes first step with gate edge cleared",
                .events_ =
                    {
                        {
                            set_running(true),
                            set_gate(true), // was low
                            set_first(3, StepEvent::Generated),
                        },
                    },
                .checks_ =
                    {
                        assert_output(original_output),
                        assert_step(3),          //
                        assert_gate(high_latch), // edge was cleared
                    },
            },
            {
                .name_ = "on GATE rise: executes nothing if no first step",
                .events_ =
                    {
                        {
                            set_running(true),
                            set_gate(true), // rise
                            set_first(no_step, original_step_event),
                        },
                    },
                .checks_ =
                    {
                        assert_output(original_output),
                        assert_step(original_step),
                        assert_gate(original_gate),
                    },
            },
            {
                .name_ = "on GATE rise: executes nothing if RUN low",
                .events_ =
                    {
                        {
                            set_running(false),
                            set_gate(true),
                        },
                    },
                .checks_ =
                    {
                        assert_output(original_output),
                        assert_step(original_step),
                        assert_gate(original_gate),
                    },
            },
            {
                .name_ = "if GATE low: executes nothing if RUN low",
                .events_ =
                    {
                        {
                            set_running(false),
                            set_gate(false),
                        },
                    },
                .checks_ =
                    {
                        assert_output(original_output),
                        assert_step(original_step),
                        assert_gate(original_gate),
                    },
            },
            {
                .name_ = "if GATE low: executes nothing, even with RUN high",
                .events_ =
                    {
                        {
                            set_gate(false),
                            set_running(true),
                        },
                    },
                .checks_ =
                    {
                        assert_output(original_output),
                        assert_step(original_step),
                        assert_gate(original_gate),
                    },
            },
            {
                .name_ = "on RESET rise: "
                         "copies input to output and does not execute a step",
                .events_ =
                    {
                        {
                            set_running(true),
                            set_reset(true), // was low
                            set_input(4.567F),
                        },
                    },
                .checks_ =
                    {
                        assert_output(4.567F),
                        assert_step(original_step),
                        assert_gate(original_gate),
                    },
            },
            {
                .name_ = "if RESET low: does not send output",
                .events_ =
                    {
                        {
                            set_running(false),
                            set_reset(false),
                        },
                    },
                .checks_ =
                    {
                        assert_output(original_output),
                        assert_step(original_step),
                        assert_gate(original_gate),
                    },
            },
            {
                .name_ = "on RESET rise: does nothing if RUN low",
                .events_ =
                    {
                        {
                            set_running(false),
                            set_reset(true), // was low
                            set_input(1.234F),
                        },
                    },
                .checks_ =
                    {
                        assert_output(original_output),
                        assert_step(original_step),
                        assert_gate(original_gate),
                    },
            },
            {
                .name_ = "RUN high RESET low: does nothing",
                .events_ =
                    {
                        {
                            set_running(true),
                            set_reset(false),
                        },
                    },
                .checks_ =
                    {
                        assert_output(original_output),
                        assert_step(original_step),
                        assert_gate(original_gate),
                    },
            },
        },
};

// PAUSED: Step in progress, but did not execute on previous process()
auto paused_tests = StateSuite{
    .name_ = "Paused",
    .events_ =
        {
            activate_step(2),     //
            {set_running(false)}, // Pause
        },
    .tests_ =
        {
            {
                .name_ = "run LOW gate LOW: does nothing",
                .events_ =
                    {
                        {set_running(false), set_gate(false)},
                    },
                .checks_ =
                    {
                        {assert_step(2)},
                        {assert_output(original_output)},
                    },
            },
            {
                .name_ = "run LOW gate RISE: does nothing",
                .events_ =
                    {
                        {set_running(false), set_gate(true)},
                    },
                .checks_ =
                    {
                        {assert_step(2)},
                        {assert_output(original_output)},
                    },
            },
            {
                .name_ = "run LOW reset LOW: does nothing",
                .events_ =
                    {
                        {set_running(false), set_reset(false)},
                    },
                .checks_ =
                    {
                        {assert_step(2)},
                        {assert_output(original_output)},
                    },
            },
            {
                .name_ = "run LOW reset RISE: becomes idle",
                .events_ =
                    {
                        {set_running(false), set_reset(true)},
                    },
                .checks_ = {assert_exited()},
            },
        },
};

// ACTIVE: Step in progress, previous execute() executed a step
auto active_tests = StateSuite{
    .name_ = "Active",
    .events_ =
        {
            activate_step(2),
        },
    .tests_ =
        {
            {.name_ = "when first activated",
             .events_ = {},
             .checks_ =
                 {
                     assert_step(2),
                     assert_gate(high_latch),
                 }},
            {
                .name_ = "executes active step with gate state",
                .events_ =
                    {
                        {
                            set_running(true),
                            set_gate(true),
                        },
                    },
                .checks_ = {assert_step(2), assert_gate(high_latch)},
            },
        },
};

/**
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
  CurveSequencerSuite() : Suite{"dhe::curve_sequencer::Engine"} {}
  void run(Tester &t) override {
    newly_constructed_tests.run(t);
    idle_tests.run(t);
    active_tests.run(t);
    paused_tests.run(t);
  }
};

void StateTest::run(Tester &t, Module &m, Engine &engine) const {
  t.run(name_, [this, &m, &engine](Tester &t) {
    for (auto const &event : events_) {
      event.apply(m);
      engine.execute(0.F);
    }
    for (auto const &check : checks_) {
      check(t, m);
    }
  });
}

void StateSuite::run(Tester &t) const {
  t.run(name_, [this](Tester &t) {
    for (auto const &test : tests_) {
      auto controls = Controls{};
      auto step_selector = StepSelector{};
      auto step_controller = StepController{};

      auto module = Module{controls, step_selector, step_controller};

      auto engine = Engine{controls, step_selector, step_controller};

      // To set cs's initial state, apply each of the suite's events.
      for (auto const &event : events_) {
        event.apply(module);
        engine.execute(0.F);
      }
      test.run(t, module, engine);
    }
  });
}

static auto _ = CurveSequencerSuite{};
} // namespace curve_sequencer
} // namespace test
