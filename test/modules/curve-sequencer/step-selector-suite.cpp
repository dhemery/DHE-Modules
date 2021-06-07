#include "modules/curve-sequencer/step-selector.h"

#include <dheunit/test.h>

#include "fixtures/step-selector-controls.h"

#include <string>
#include <utility>
#include <vector>

namespace test {
namespace curve_sequencer {
static auto constexpr step_count = 8;
using Controls = StepSelectorControls<step_count>;
using StepSelector = dhe::curve_sequencer::StepSelector<Controls>;

using dhe::unit::Suite;
using dhe::unit::Tester;

struct SuccessorTest {
  std::string name_;               // NOLINT
  std::pair<int, int> selection_;  // NOLINT
  std::vector<int> enabled_steps_; // NOLINT
  int current_step_;               // NOLINT
  bool looping_;                   // NOLINT
  int want_;                       // NOLINT

  void run(Tester &t) const;
};

static auto const successor_tests = std::vector<SuccessorTest>{
    {
        .name_ = "is next step if all steps are enabled and selected",
        .selection_ = {0, step_count},              // 0 1 2 3 4 5 6 7
        .enabled_steps_ = {0, 1, 2, 3, 4, 5, 6, 7}, // all enabled
        .current_step_ = 2,
        .looping_ = false,
        .want_ = 3,
    },
    {
        .name_ = "skips disabled steps",
        .selection_ = {0, step_count},     // 0 1 2 3 4 5 6 7
        .enabled_steps_ = {0, 1, 2, 6, 7}, // 0 1 2 x x x 6 7
        .current_step_ = 2,
        .looping_ = false,
        .want_ = 6, // skipped disabled 3, 4, and 5
    },
    {
        .name_ = "wraps search if selection wraps",
        .selection_ = {7, 7},                       // 0 1 2 3 4 5 x 7 (wraps)
        .enabled_steps_ = {0, 1, 2, 3, 4, 5, 6, 7}, // 0 1 2 3 4 5 6 7
        .current_step_ = 7,                         // no higher steps
        .looping_ = false,
        .want_ = 0, // below current step, so can be found only by wrapping
    },
    {
        .name_ = "skips disabled steps when wrapping",
        .selection_ = {4, 7},        // 0 1 2 x 4 5 6 7 (wraps)
        .enabled_steps_ = {2, 3, 4}, // x x 2 3 4 x x x
        .current_step_ = 4,          // highest enabled selected step
        .looping_ = false,
        .want_ = 2, // lowest enabled selected step
    },
    {
        .name_ = "is selection end if selection end is next enabled step",
        .selection_ = {2, 3},        // x x 2 3 4 x x x
        .enabled_steps_ = {2, 3, 4}, // x x 2 3 4 x x x
        .current_step_ = 3,
        .looping_ = false,
        .want_ = 4,
    },
    {
        .name_ = "no step if all selected steps disabled",
        .selection_ = {0, 4},           // 0 1 2 3 x x x x
        .enabled_steps_ = {4, 5, 6, 7}, // x x x x 4 5 6 7
        .current_step_ = 0,
        .looping_ = true,
        .want_ = -1, // no step
    },
    {
        .name_ = "next enabled selected step even if current step is below "
                 "selection",
        .selection_ = {5, 3},              // x x x x x 5 6 7
        .enabled_steps_ = {3, 4, 5, 6, 7}, // x x x 3 x 5 6 7
        .current_step_ = 3,                // below selection
        .looping_ = true,
        .want_ = 5,
    },
    {
        .name_ = "next enabled selected step even if current step is above "
                 "selection",
        .selection_ = {2, 4},     // x 1 2 3 4 x x x
        .enabled_steps_ = {2, 3}, // x x 2 3 x x x x
        .current_step_ = 5,       // above selection
        .looping_ = true,
        .want_ = 2,
    },
    {
        .name_ = "next enabled selected step even if current step is not in "
                 "wrapped selection",
        .selection_ = {6, 4},  // 0 1 2 3 x x 6 7
        .enabled_steps_ = {3}, // x x x 3 x x x x
        .current_step_ = 5,    // above selection end, below selection start
        .looping_ = true,
        .want_ = -1,
    },
    {
        .name_ = "current step if next enabled selected step and sequencer is "
                 "looping",
        .selection_ = {0, step_count}, // 0 1 2 3 4 5 6 7
        .enabled_steps_ = {3},         // x x x 3 x x x x
        .current_step_ = 3,            // The only enabled, selected step
        .looping_ = true,
        .want_ = 3,
    },
    {
        .name_ = "selection end has no successor if sequencer is not looping",
        .selection_ = {1, 3},                       // x 1 2 3 x x x x
        .enabled_steps_ = {0, 1, 2, 3, 4, 5, 6, 7}, // 0 1 2 3 4 5 6 7
        .current_step_ = 3,                         // selection end
        .looping_ = false,
        .want_ = -1, // no step because not looping
    },
};

struct StepSelectorSuite : public Suite {
  StepSelectorSuite() : Suite{"curve_sequencer::StepSelector"} {}

  void run(Tester &t) override {
    for (auto const &test : successor_tests) {
      test.run(t);
    }
  }
};

void SuccessorTest::run(Tester &t) const {
  t.run(name_, [this](Tester &t) {
    auto controls = Controls{};
    controls.selection_ = selection_;
    controls.selection_ = selection_;
    for (auto const s : enabled_steps_) {
      controls.is_enabled_[s] = true;
    }

    auto selector = StepSelector{controls, step_count};

    auto const got = selector.successor(current_step_, looping_);

    if (got != want_) {
      t.errorf("Got {}, want {}", got, want_);
    }
  });
}

static auto _ = StepSelectorSuite{};
} // namespace curve_sequencer
} // namespace test
