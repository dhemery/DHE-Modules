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
  std::string name_;
  SetConditions setup_; // NOLINT
  Check check_;         // NOLINT
  void run(Tester &t, Context &context, CurveSequencer &curve_sequencer) const;
};

struct StateSuite {
  std::string name_;
  SetState enter_state_; // NOLINT
  std::vector<StateTest> tests_;
  void run(Tester &t) const;
};

// CurveSequencer starts idle, so this is a no-op.
static inline void enter_idle_mode(Context &context,
                                   CurveSequencer &curve_sequencer) {}

// Note: Context throws if a command is called without being explicitly allowed
auto idle_tests = StateSuite{
    .name_ = "Idle",
    .enter_state_ = enter_idle_mode,
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

struct CurveSequencerSuite : public Suite {
  CurveSequencerSuite() : Suite{"dhe::curve_suquencer::CurveSequencer"} {}
  void run(Tester &t) override { idle_tests.run(t); }
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

static auto _ = CurveSequencerSuite{};
} // namespace curve_sequencer
} // namespace test
