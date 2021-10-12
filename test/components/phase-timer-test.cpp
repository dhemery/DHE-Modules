#include "components/phase-timer.h"

#include "dheunit/test.h"

#include <functional>
#include <vector>

namespace dhe {
namespace components {
namespace test {
using dhe::PhaseTimer;
using dhe::unit::Suite;
using dhe::unit::Tester;

struct PhaseTimerTest {
  std::string const name_;                      // NOLINT
  PhaseTimer const timer_;                      // NOLINT
  std::function<void(PhaseTimer &t)> const op_; // NOLINT
  float const want_phase_;                      // NOLINT
  bool const want_in_progress_;                 // NOLINT

  void run(Tester &t) {
    t.run(name_, [this](Tester &t) {
      auto timer = timer_;
      op_(timer);

      auto const phase = timer.phase();
      if (phase != want_phase_) {
        t.errorf("Got {}, want {}", phase, want_phase_);
      }

      auto const in_progress = timer.in_progress();
      if (in_progress != want_in_progress_) {
        t.errorf("in_progress() returned {}, want {}", in_progress,
                 want_in_progress_);
      }
    });
  }
};

static std::vector<PhaseTimerTest> phase_timer_tests = {
    {
        .name_ = "default timer is in progress at phase 0",
        .timer_ = PhaseTimer{},
        .op_ = [](PhaseTimer &p) {},
        .want_phase_ = 0.F,
        .want_in_progress_ = true,
    },
    {
        .name_ = "remembers initial phase",
        .timer_ = PhaseTimer{0.934F},
        .op_ = [](PhaseTimer &p) {},
        .want_phase_ = 0.934F,
        .want_in_progress_ = true,
    },
    {
        .name_ = "timer initialized with excess phase caps phase at 1",
        .timer_ = PhaseTimer{22.F},
        .op_ = [](PhaseTimer &p) {},
        .want_phase_ = 1.F,
        .want_in_progress_ = false,
    },
    {
        .name_ = "advance() advances phase",
        .timer_ = PhaseTimer{},
        .op_ =
            [](PhaseTimer &p) {
              p.advance(0.124F);
              p.advance(0.381F);
            },
        .want_phase_ = 0.124F + 0.381F,
        .want_in_progress_ = true,
    },
    {
        .name_ = "advance() caps phase at 1",
        .timer_ = PhaseTimer{},
        .op_ = [](PhaseTimer &p) { p.advance(3.F); },
        .want_phase_ = 1.F,
        .want_in_progress_ = false,
    },
    {
        .name_ = "reset() resumes progress at phase 0",
        .timer_ = PhaseTimer{},
        .op_ =
            [](PhaseTimer &p) {
              p.advance(9.F);
              p.reset();
            },
        .want_phase_ = 0.F,
        .want_in_progress_ = true,
    },
};

class PhaseTimerSuite : public Suite {
public:
  PhaseTimerSuite() : Suite{"dhe::PhaseTimer"} {}

  void run(Tester &t) override {
    for (auto &test : phase_timer_tests) {
      test.run(t);
    }
  }
};

static auto _ = PhaseTimerSuite{};
} // namespace test
} // namespace components
} // namespace dhe
