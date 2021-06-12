#include "modules/curve-sequencer/curve-sequencer.h"

#include "fixtures/curve-sequencer-fixture.h"

#include <dheunit/test.h>

#include <functional>
#include <string>
#include <vector>

namespace test {
namespace curve_sequencer {
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::high_latch;

// TODO: Test that cs starts idle

struct StateTest {
  std::string name_;
  SetConditions set_conditions_; // NOLINT
  Check check_;                  // NOLINT
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

auto idle_tests = StateSuite{
    .name_ = "Idle",
    .enter_state_ = enter_idle_mode,
    .tests_ =
        {
            {
                .name_ = "running and reset: copy input to output",
                .set_conditions_ =
                    [](Context &context) {
                      context.controls_.is_running_ = true;
                      context.controls_.is_reset_ = true;
                      context.controls_.input_ = 3.9434F;
                      context.controls_.allow_output_ = true;
                    },
                .check_ =
                    [](Context &context) {
                      context.controls_.check_output(3.9434F);
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
    set_conditions_(context);
    curve_sequencer.execute(0.1F);
    check_(context);
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
      test.run(t, context, curve_sequencer);
    }
  });
}

static auto _ = CurveSequencerSuite{};
} // namespace curve_sequencer
} // namespace test
