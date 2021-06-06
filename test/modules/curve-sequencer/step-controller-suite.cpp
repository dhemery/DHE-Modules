#include "components/sigmoid.h"
#include "fixtures/advance-mode-enums.h"
#include "fixtures/controls-fixture.h"
#include "fixtures/step-event-enums.h"
#include "helpers/latches.h"
#include "modules/curve-sequencer/advance-mode.h"
#include "modules/curve-sequencer/step-controller.h"
#include "modules/curve-sequencer/step-event.h"

#include <array>
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
using dhe::sigmoid::Taper;
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::falling_latch;
using test::high_latch;
using test::low_latch;
using test::rising_latch;
using test::curve_sequencer::Controls;

struct FakeControls {
  FakeControls() {
    for (int i = 0; i < step_count; i++) {
      taper_[i] = &dhe::sigmoid::s_taper;
    }
  }
  float input_;                                        // NOLINT
  float output_;                                       // NOLINT
  std::array<AdvanceMode, step_count> advance_mode_;   // NOLINT
  std::array<float, step_count> curvature_;            // NOLINT
  std::array<float, step_count> duration_;             // NOLINT
  std::array<GenerateMode, step_count> generate_mode_; // NOLINT
  std::array<float, step_count> level_;                // NOLINT
  std::array<float, step_count> progress_;             // NOLINT
  std::array<Taper const *, step_count> taper_;        // NOLINT

  auto condition(int s) -> AdvanceMode { return advance_mode_[s]; }
  auto curvature(int s) -> float { return curvature_[s]; }
  auto duration(int s) -> float { return duration_[s]; }
  auto input() -> float { return input_; }
  auto level(int s) -> float { return level_[s]; }
  auto mode(int s) -> GenerateMode { return generate_mode_[s]; }
  void output(float v) { output_ = v; }
  auto output() -> float { return output_; }
  void show_inactive(int s) { progress_[s] = 0.F; }
  void show_progress(int s, float p) { progress_[s] = p; }
  auto taper(int s) -> Taper const & { return *taper_[s]; }
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

// TODO: advance on time returns generated if timer does not expire
// TODO: advance on time returns completed if timer expires
// TODO: test generate

struct StepControllerSuite : public Suite {
  StepControllerSuite() : Suite{"dhe::curve_sequencer::StepController"} {}

  void run(Tester &t) override {
    run_advance_mode_tests(t);

    t.run("enter(step) activates step at 0 progress", [](Tester &t) {
      auto const step = std::rand() % step_count;
      auto controls = FakeControls{};
      auto timer = PhaseTimer{};
      auto step_controller = StepController{controls, timer};

      step_controller.enter(step);

      auto const got = controls.progress_[step];
      auto constexpr want = 0.F;

      if (got != want) {
        t.errorf("Got step {} progress {}, want {}", step, got, want);
      }
    });
  }
};

// TODO: Pick a step, set Controls to initialize only that step, and enter the
//  step before executing.
void run_advance_mode_tests(Tester &t) {
  t.run("AdvanceMode", [](Tester &t) {
    for (auto &mode_tests : advance_mode_tests) {
      auto const mode = mode_tests.first;
      auto const tests = mode_tests.second;
      t.run(name_of(mode), [&mode, &tests](Tester &t) {
        for (auto test : tests) {
          t.run(test::name_of(test.gate_), [&mode, &test](Tester &t) {
            auto const step = std::rand() % step_count;
            auto timer = PhaseTimer{};
            auto controls = FakeControls{};
            controls.advance_mode_[step] = mode;
            controls.duration_[step] = 10.F;

            auto step_controller = StepController{controls, timer};
            step_controller.enter(step);

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
