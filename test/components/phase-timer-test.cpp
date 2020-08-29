#include "components/phase-timer.h"

#include "dheunit/assertions.h"
#include "dheunit/test.h"

namespace dhe {
namespace components {
namespace test {
using dhe::PhaseTimer;
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;

class PhaseTimerSuite : public Suite {
public:
  PhaseTimerSuite() : Suite{"dhe::PhaseTimer"} {}

  void register_tests(unit::TestRegistrar add_test) override {

    add_test("default is in progress at phase 0", [](Tester &t) {
      auto timer = PhaseTimer{};
      t.assert_that("phase", timer.phase(), is_equal_to(0.F));
      t.assert_that("in progress", timer.in_progress(), is_true);
    });

    add_test("remembers starting phase", [](Tester &t) {
      auto starting_phase = 0.215F;
      auto timer = PhaseTimer{starting_phase};
      t.assert_that(timer.phase(), is_equal_to(starting_phase));
    });

    add_test("advance adds delta to phase", [](Tester &t) mutable {
      auto timer = PhaseTimer{};
      auto constexpr delta = 0.38F;

      timer.advance(delta);
      t.assert_that("after first advance", timer.phase(), is_equal_to(delta));

      timer.advance(delta);
      t.assert_that("after second advance", timer.phase(),
                    is_equal_to(delta + delta));
    });

    add_test("reset sets to in progress at phase 0", [](Tester &t) {
      auto timer = PhaseTimer{0.999F};
      timer.reset();
      t.assert_that("phase", timer.phase(), is_equal_to(0.F));
      t.assert_that("in progress", timer.in_progress(), is_true);
    });

    add_test("maximum phase is 1", [](Tester &t) {
      auto timer = PhaseTimer{1234.56789F};
      t.assert_that("constructed with excessive phase", timer.phase(),
                    is_equal_to(1.F));

      timer.advance(9999.234F);
      t.assert_that("advanced with excessive delta", timer.phase(),
                    is_equal_to(1.F));
    });

    add_test("in progress iff phase less than 1", [](Tester &t) {
      auto timer = PhaseTimer{};

      timer.advance(0.99999F);
      t.assert_that("phase just below 1", timer.in_progress(), is_false);

      timer.advance(3.F);
      t.assert_that("phase advanced to 1", timer.in_progress(), is_true);
    });
  }
};

__attribute__((unused)) static PhaseTimerSuite phase_timer_suite{};
} // namespace test
} // namespace components
} // namespace dhe
