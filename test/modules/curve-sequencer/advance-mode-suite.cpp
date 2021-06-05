#include "fixtures/advance-mode-enums.h"
#include "fixtures/step-event-enums.h"
#include "helpers/latches.h"
#include "modules/curve-sequencer/advance-mode.h"
#include "modules/curve-sequencer/step-controller.h"
#include "modules/curve-sequencer/step-event.h"

#include <dheunit/test.h>
#include <map>
#include <vector>

using dhe::Latch;
using dhe::curve_sequencer::AdvanceMode;
using dhe::curve_sequencer::StepEvent;
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::falling_latch;
using test::high_latch;
using test::low_latch;
using test::rising_latch;

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

void run_advance_mode_tests(Tester &t) {
  t.run("AdvanceMode", [](Tester &t) {
    for (auto &mode_tests : advance_mode_tests) {
      auto const mode = mode_tests.first;
      auto const tests = mode_tests.second;
      t.run(test::curve_sequencer::name_of(mode), [&mode, &tests](Tester &t) {
        for (auto test : tests) {
          t.run("foo", [&test](Tester &t) {
            auto constexpr got = StepEvent::Generated;
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
