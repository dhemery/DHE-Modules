#include "fixtures/advance-mode-enums.h"
#include "fixtures/controls-fixture.h"
#include "fixtures/step-event-enums.h"
#include "helpers/latches.h"
#include "modules/curve-sequencer/advance-mode.h"
#include "modules/curve-sequencer/step-controller.h"
#include "modules/curve-sequencer/step-event.h"

#include <dheunit/test.h>
#include <map>
#include <vector>

namespace test {
namespace curve_sequencer {
using dhe::Latch;
using dhe::PhaseTimer;
using dhe::curve_sequencer::AdvanceMode;
using dhe::curve_sequencer::GenerateMode;
using dhe::curve_sequencer::StepEvent;
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::falling_latch;
using test::high_latch;
using test::low_latch;
using test::rising_latch;
using test::curve_sequencer::Controls;

struct FakeControls {
  AdvanceMode advance_mode_;
  void show_progress(int, float) {}
  void show_inactive(int) {}
  void output(float) {}
  auto input() -> float { return 5.F; }
  auto mode(int) -> GenerateMode { return GenerateMode::Curve; }
  auto condition(int) -> AdvanceMode { return advance_mode_; }
  auto duration(int) -> float { return 10.F; }
  auto curvature(int) -> float { return 05.F; }
  auto taper(int) -> dhe::sigmoid::Taper const & {
    return dhe::sigmoid::j_taper;
  }
  auto level(int) -> float { return 0.F; }
};
using StepController = dhe::curve_sequencer::StepController<FakeControls>;

struct AdvanceModeTest {
  Latch gate_;
  StepEvent want_;
};

auto advance_mode_tests = std::map<AdvanceMode, std::vector<AdvanceModeTest>>{
    {
        AdvanceMode::GateIsHigh,
        {{.gate_ = high_latch, .want_ = StepEvent::Completed},
         {.gate_ = low_latch, .want_ = StepEvent::Generated},
         {.gate_ = rising_latch, .want_ = StepEvent::Completed},
         {.gate_ = falling_latch, .want_ = StepEvent::Generated}},
    },
    {
        AdvanceMode::GateIsLow,
        {{.gate_ = high_latch, .want_ = StepEvent::Generated},
         {.gate_ = low_latch, .want_ = StepEvent::Completed},
         {.gate_ = rising_latch, .want_ = StepEvent::Generated},
         {.gate_ = falling_latch, .want_ = StepEvent::Completed}},
    },
    {
        AdvanceMode::GateRises,
        {{.gate_ = high_latch, .want_ = StepEvent::Generated},
         {.gate_ = low_latch, .want_ = StepEvent::Generated},
         {.gate_ = rising_latch, .want_ = StepEvent::Completed},
         {.gate_ = falling_latch, .want_ = StepEvent::Generated}},
    },
    {
        AdvanceMode::GateFalls,
        {{.gate_ = high_latch, .want_ = StepEvent::Generated},
         {.gate_ = low_latch, .want_ = StepEvent::Generated},
         {.gate_ = rising_latch, .want_ = StepEvent::Generated},
         {.gate_ = falling_latch, .want_ = StepEvent::Completed}},
    },
    {
        AdvanceMode::GateChanges,
        {{.gate_ = high_latch, .want_ = StepEvent::Generated},
         {.gate_ = low_latch, .want_ = StepEvent::Generated},
         {.gate_ = rising_latch, .want_ = StepEvent::Completed},
         {.gate_ = falling_latch, .want_ = StepEvent::Completed}},
    },
};

void run_advance_mode_tests(Tester &t);

struct StepControllerSuite : public Suite {
  StepControllerSuite() : Suite{"dhe::curve_sequencer::StepController"} {}

  void run(Tester &t) override { run_advance_mode_tests(t); }
};

// TODO: Pick a step, set Controls to initialize only that step, and enter the
//  step before executing.
// TODO: Test for advance on timer expiration.
void run_advance_mode_tests(Tester &t) {
  t.run("AdvanceMode", [](Tester &t) {
    for (auto &mode_tests : advance_mode_tests) {
      auto const mode = mode_tests.first;
      auto const tests = mode_tests.second;
      t.run(name_of(mode), [&mode, &tests](Tester &t) {
        for (auto test : tests) {
          t.run(test::name_of(test.gate_), [&mode, &test](Tester &t) {
            auto controls = FakeControls{};
            controls.advance_mode_ = mode;
            auto timer = PhaseTimer{};
            auto step_controller = StepController{controls, timer};
            auto const got = step_controller.execute(test.gate_, 0.1F);
            if (got != test.want_) {
              t.errorf("Got {}, want {}", got, test.want_);
            }
          });
        }
      });
    }
  });
}

static auto _ = StepControllerSuite{};
} // namespace curve_sequencer
} // namespace test