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
  std::string name_;
  std::function<PhaseTimer()> init_;
  std::function<void(PhaseTimer &t)> op_;
  float want_phase_;
  bool want_in_progress_;
};

static std::vector<PhaseTimerTest> phase_timer_tests = {
    {
        "defaults to in progress at phase 0",        //
        []() -> PhaseTimer { return PhaseTimer{}; }, //
        [](PhaseTimer &p) {},                        //
        0.F,                                         //
        true,                                        //
    },
    {
        "remembers starting phase",                        //
        []() -> PhaseTimer { return PhaseTimer{0.934F}; }, //
        [](PhaseTimer &p) {},                              //
        0.934F,                                            //
        true,                                              //
    },
    {
        "constructor stops progress if phase >= 1",      //
        []() -> PhaseTimer { return PhaseTimer{22.F}; }, //
        [](PhaseTimer &p) {},                            //
        1.F,                                             //
        false,                                           //
    },
    {
        "advance() advances phase",                  //
        []() -> PhaseTimer { return PhaseTimer{}; }, //
        [](PhaseTimer &p) {
          p.advance(0.124F);
          p.advance(0.381F);
        },
        0.124F + 0.381F, //
        true,
    },
    {
        "advance() stops progress if phase >= 1",    //
        []() -> PhaseTimer { return PhaseTimer{}; }, //
        [](PhaseTimer &p) { p.advance(3.F); },       //
        1.F,                                         //
        false,                                       //
    },
    {
        "reset() resumes progress at phase 0",       //
        []() -> PhaseTimer { return PhaseTimer{}; }, //
        [](PhaseTimer &p) {
          p.advance(9.F);
          p.reset();
        },
        0.F,
        true,
    },
};

class PhaseTimerSuite : public Suite {
public:
  PhaseTimerSuite() : Suite{"dhe::PhaseTimer"} {}

  void run(Tester &t) override {
    for (auto &test : phase_timer_tests) {
      t.run(test.name_, [test](Tester &t) {
        auto timer = test.init_();
        test.op_(timer);
        auto const phase = timer.phase();
        if (phase != test.want_phase_) {
          t.errorf("phase() returned {}, want {}", phase, test.want_phase_);
        }
        auto const in_progress = timer.in_progress();
        if (in_progress != test.want_in_progress_) {
          t.errorf("in_progress() returned {}, want {}", in_progress,
                   test.want_in_progress_);
        }
      });
    }
  }
};

__attribute__((unused)) static auto _ = PhaseTimerSuite{};
} // namespace test
} // namespace components
} // namespace dhe
