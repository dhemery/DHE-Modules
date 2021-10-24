#include "modules/curve-sequencer/advance-mode.h"
#include "modules/curve-sequencer/step-controller.h"
#include "modules/curve-sequencer/step-event.h"

#include "components/sigmoid.h"

#include "./fixtures/advance-mode-enums.h"
#include "./fixtures/step-controller-signals.h"
#include "./fixtures/step-event-enums.h"

#include "helpers/latches.h"

#include "dheunit/test.h"

#include <array>
#include <vector>

namespace test {
namespace curve_sequencer {
static auto constexpr step_count = 8;

using dhe::curve_sequencer::AdvanceMode;
using dhe::curve_sequencer::GenerateMode;
using dhe::curve_sequencer::StepEvent;
using Signals = StepControllerSignals<step_count>;
using StepController = dhe::curve_sequencer::StepController<Signals>;

using dhe::Latch;
using dhe::PhaseTimer;
using dhe::sigmoid::Taper;

using test::falling_latch;
using test::high_latch;
using test::low_latch;
using test::rising_latch;

using dhe::unit::Suite;
using dhe::unit::Tester;

struct AdvanceModeTest {
  Latch gate_;     // NOLINT
  StepEvent want_; // NOLINT

  void run(Tester &t, AdvanceMode mode) const;
};

struct AdvanceModeSuite {
  AdvanceMode mode_;                   // NOLINT
  std::vector<AdvanceModeTest> tests_; // NOLINT

  void run(Tester &t) const;
};

auto advance_mode_suites = std::vector<AdvanceModeSuite>{
    {
        .mode_ = AdvanceMode::GateIsHigh,
        .tests_ = {{.gate_ = high_latch, .want_ = StepEvent::Completed},
                   {.gate_ = low_latch, .want_ = StepEvent::Generated},
                   {.gate_ = rising_latch, .want_ = StepEvent::Completed},
                   {.gate_ = falling_latch, .want_ = StepEvent::Generated}},
    },
    {
        .mode_ = AdvanceMode::GateIsLow,
        .tests_ = {{.gate_ = high_latch, .want_ = StepEvent::Generated},
                   {.gate_ = low_latch, .want_ = StepEvent::Completed},
                   {.gate_ = rising_latch, .want_ = StepEvent::Generated},
                   {.gate_ = falling_latch, .want_ = StepEvent::Completed}},
    },
    {
        .mode_ = AdvanceMode::GateRises,
        .tests_ = {{.gate_ = high_latch, .want_ = StepEvent::Generated},
                   {.gate_ = low_latch, .want_ = StepEvent::Generated},
                   {.gate_ = rising_latch, .want_ = StepEvent::Completed},
                   {.gate_ = falling_latch, .want_ = StepEvent::Generated}},
    },
    {
        .mode_ = AdvanceMode::GateFalls,
        .tests_ = {{.gate_ = high_latch, .want_ = StepEvent::Generated},
                   {.gate_ = low_latch, .want_ = StepEvent::Generated},
                   {.gate_ = rising_latch, .want_ = StepEvent::Generated},
                   {.gate_ = falling_latch, .want_ = StepEvent::Completed}},
    },
    {
        .mode_ = AdvanceMode::GateChanges,
        .tests_ = {{.gate_ = high_latch, .want_ = StepEvent::Generated},
                   {.gate_ = low_latch, .want_ = StepEvent::Generated},
                   {.gate_ = rising_latch, .want_ = StepEvent::Completed},
                   {.gate_ = falling_latch, .want_ = StepEvent::Completed}},
    },
};

// TODO: advance on time returns generated if timer does not expire
// TODO: advance on time returns completed if timer expires
// TODO: test generate

struct StepControllerSuite : public Suite {
  StepControllerSuite() : Suite{"dhe::curve_sequencer::StepController"} {}

  void run(Tester &t) override {
    t.run("enter(step) activates step at 0 progress", [](Tester &t) {
      auto const step = std::rand() % step_count;
      auto signals = Signals{};
      auto timer = PhaseTimer{};
      auto step_controller = StepController{signals, timer};

      step_controller.enter(step);

      auto const got = signals.progress_[step];
      auto constexpr want = 0.F;

      if (got != want) {
        t.errorf("Got step {} progress {}, want {}", step, got, want);
      }
    });

    t.run("exit() deactivates current step", [](Tester &t) {
      auto const step = std::rand() % step_count;
      auto signals = Signals{};
      auto timer = PhaseTimer{};
      auto step_controller = StepController{signals, timer};

      step_controller.enter(step);
      step_controller.exit();

      if (signals.deactivated_step_ != step) {
        t.errorf("Got deactivated step {}, want {}", signals.deactivated_step_,
                 step);
      }
    });

    t.run("AdvanceModes", [](Tester &t) {
      for (auto &s : advance_mode_suites) {
        s.run(t);
      }
    });
  }
};

void AdvanceModeTest::run(Tester &t, AdvanceMode mode) const {
  t.run(test::name_of(gate_), [this, mode](Tester &t) {
    auto signals = Signals{};
    auto timer = PhaseTimer{};
    auto controller = StepController{signals, timer};

    auto const step = std::rand() % step_count;
    signals.advance_mode_[step] = mode;
    signals.duration_[step] = 10.F;

    controller.enter(step);

    auto const got = controller.execute(gate_, 0.1F);

    if (got != want_) {
      t.errorf("Got {}, want {}", got, want_);
    }
  });
}

void AdvanceModeSuite::run(Tester &t) const {
  t.run(name_of(mode_), [this](Tester &t) {
    for (auto const &test : tests_) {
      test.run(t, mode_);
    }
  });
}

static auto _ = StepControllerSuite{};
} // namespace curve_sequencer
} // namespace test
