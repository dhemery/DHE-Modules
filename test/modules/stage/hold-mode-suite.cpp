#include "./fixtures/timed-mode-fixture.h"
#include "components/latch.h"
#include "helpers/latches.h"
#include "modules/stage/event.h"
#include "modules/stage/hold-mode.h"

#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace stage {

using dhe::Latch;
using dhe::PhaseTimer;
using dhe::stage::Event;

using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestRegistrar;

using HoldMode = dhe::stage::HoldMode<Controls, PhaseTimer>;

class HoldModeSuite : public Suite {
public:
  HoldModeSuite() : Suite{"dhe::stage::HoldMode"} {}
  void register_tests(TestRegistrar add) override {
    add("enter() activates stage",
        test<HoldMode>([](Tester &t, Controls &controls, PhaseTimer & /**/,
                          HoldMode &mode) {
          controls.active_ = false;
          mode.enter();
          t.assert_that(controls.active_, is_true);
        }));

    add("enter() copies input to output",
        test<HoldMode>([](Tester &t, Controls &controls, PhaseTimer & /**/,
                          HoldMode &mode) {
          auto constexpr input = 0.234F;
          controls.input_ = input;
          mode.enter();
          t.assert_that(controls.output_, is_equal_to(input));
        }));

    add("enter() resets timer",
        test<HoldMode>(
            [](Tester &t, Controls & /**/, PhaseTimer &timer, HoldMode &mode) {
              timer.advance(1.F);
              mode.enter();
              t.assert_that(timer.phase(), is_equal_to(0.F));
            }));

    add("execute(l,s) resets timer if latch rises",
        test<HoldMode>([](Tester &t, Controls &controls, PhaseTimer &timer,
                          HoldMode &mode) {
          timer.reset();
          timer.advance(0.9F); // nearly complete before executing
          controls.duration_ = 1.F;
          auto constexpr sample_time = 0.F; // Cheat: Advance by 0 after reset
          mode.execute(rising_latch, sample_time);
          t.assert_that(timer.phase(), is_equal_to(0.F));
        }));

    add("execute(l,s) reports generated if timer not expired",
        test<HoldMode>([](Tester &t, Controls &controls, PhaseTimer &timer,
                          HoldMode &mode) {
          timer.reset();
          controls.duration_ = 1.F;
          auto constexpr sample_time = 0.1F;
          auto const result = mode.execute(low_latch, sample_time);
          t.assert_that(result, is_equal_to(Event::Generated));
        }));

    add("execute(l,s) reports completed if timer expires",
        test<HoldMode>([](Tester &t, Controls &controls, PhaseTimer &timer,
                          HoldMode &mode) {
          timer.advance(0.99999F);
          controls.duration_ = 1.F;
          auto constexpr sample_time = 0.1F; // Enough to advance to 1
          auto const result = mode.execute(low_latch, sample_time);
          t.assert_that(result, is_equal_to(Event::Completed));
        }));

    add("execute(l,s) outputs nothing",
        test<HoldMode>([](Tester &t, Controls &controls, PhaseTimer & /**/,
                          HoldMode &mode) {
          float original_output = 99.F;
          controls.output_ = original_output;
          mode.execute(low_latch, 0.1F);
          t.assert_that(controls.output_, is_equal_to(original_output));
        }));

    add("exit() deactivates stage",
        test<HoldMode>([](Tester &t, Controls &controls, PhaseTimer & /**/,
                          HoldMode &mode) {
          controls.active_ = true;
          mode.exit();
          t.assert_that(controls.active_, is_false);
        }));
  }
};

__attribute__((unused)) static auto _ = HoldModeSuite{};

} // namespace stage
}; // namespace test
