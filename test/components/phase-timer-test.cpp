#include "components/phase-timer.h"

#include "dheunit/test.h"

namespace dhe {
namespace components {
namespace test {
using dhe::PhaseTimer;
using dhe::unit::Suite;
using dhe::unit::Tester;

class PhaseTimerSuite : public Suite {
public:
  PhaseTimerSuite() : Suite{"dhe::PhaseTimer"} {}

  void register_tests(unit::TestRegistrar add_test) override {

    add_test("default is in progress at phase 0", [](Tester &t) {
      auto timer = PhaseTimer{};

      if (timer.phase() != 0.F) {
        t.errorf("got phase {}, want {}", timer.phase(), 0.F);
      }

      if (!timer.in_progress()) {
        t.errorf("got in progress false, want true");
      }
    });

    add_test("remembers starting phase", [](Tester &t) {
      auto starting_phase = 0.215F;

      auto timer = PhaseTimer{starting_phase};

      if (timer.phase() != starting_phase) {
        t.errorf("got phase {}, want {}", timer.phase(), starting_phase);
      }
    });

    add_test("advance adds delta to phase", [](Tester &t) mutable {
      auto timer = PhaseTimer{};
      auto constexpr delta = 0.38F;

      timer.advance(delta);

      if (timer.phase() != delta) {
        t.errorf("got phase {}, want {}", timer.phase(), delta);
      }

      timer.advance(delta);

      if (timer.phase() != delta + delta) {
        t.errorf("got phase {}, want {}", timer.phase(), delta + delta);
      }
    });

    add_test("reset sets to in progress at phase 0", [](Tester &t) {
      auto timer = PhaseTimer{0.999F};

      timer.reset();

      if (timer.phase() != 0.F) {
        t.errorf("got phase {}, want {}", timer.phase(), 1.F);
      }

      if (!timer.in_progress()) {
        t.errorf("got in progress false, want true");
      }
    });

    add_test("maximum phase is 1", [](Tester &t) {
      auto timer = PhaseTimer{1234.56789F};

      if (timer.phase() != 1.F) {
        t.errorf("got phase {}, want {}", timer.phase(), 1.F);
      }

      timer.advance(9999.234F);

      if (timer.phase() != 1.F) {
        t.errorf("got phase {}, want {}", timer.phase(), 1.F);
      }
    });

    add_test("in progress iff phase less than 1", [](Tester &t) {
      auto timer = PhaseTimer{};

      timer.advance(0.99999F);

      if (!timer.in_progress()) {
        t.errorf("got in progress false, want true");
      }

      timer.advance(3.F);

      if (timer.in_progress()) {
        t.errorf("got in progress true, want false");
      }
    });
  }
};

__attribute__((unused)) static PhaseTimerSuite phase_timer_suite{};
} // namespace test
} // namespace components
} // namespace dhe
