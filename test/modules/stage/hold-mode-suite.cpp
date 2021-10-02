#include "./fixtures/timed-mode-fixture.h"
#include "helpers/assertions.h"
#include "helpers/latches.h"
#include "modules/stage/event.h"
#include "modules/stage/hold-mode.h"

#include <dheunit/test.h>

namespace test {
namespace stage {

using dhe::PhaseTimer;
using dhe::stage::Event;

using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;
using test::is_false;
using test::is_true;

using HoldMode = dhe::stage::HoldMode<Controls, PhaseTimer>;

class HoldModeSuite : public Suite {
public:
  HoldModeSuite() : Suite{"dhe::stage::HoldMode"} {}
  void run(Tester &t) override {
    t.run("enter() activates stage",
          test<HoldMode>([](Tester &t, Controls &controls, PhaseTimer & /**/,
                            HoldMode &mode) {
            controls.active_ = false;
            mode.enter();
            assert_that(t, controls.active_, is_true);
          }));

    t.run("enter() copies input to output",
          test<HoldMode>([](Tester &t, Controls &controls, PhaseTimer & /**/,
                            HoldMode &mode) {
            auto constexpr input = 0.234F;
            controls.input_ = input;
            mode.enter();
            assert_that(t, controls.output_, is_equal_to(input));
          }));

    t.run("enter() resets timer",
          test<HoldMode>([](Tester &t, Controls & /**/, PhaseTimer &timer,
                            HoldMode &mode) {
            timer.advance(1.F);
            mode.enter();
            assert_that(t, timer.phase(), is_equal_to(0.F));
          }));

    t.run("execute(l,s) resets timer if latch rises",
          test<HoldMode>([](Tester &t, Controls &controls, PhaseTimer &timer,
                            HoldMode &mode) {
            timer.reset();
            timer.advance(0.9F); // nearly complete before executing
            controls.duration_ = 1.F;
            auto constexpr sample_time = 0.F; // Cheat: Advance by 0 after reset
            mode.execute(rising_latch, sample_time);
            assert_that(t, timer.phase(), is_equal_to(0.F));
          }));

    t.run("execute(l,s) reports generated if timer not expired",
          test<HoldMode>([](Tester &t, Controls &controls, PhaseTimer &timer,
                            HoldMode &mode) {
            timer.reset();
            controls.duration_ = 1.F;
            auto constexpr sample_time = 0.1F;
            auto const result = mode.execute(low_latch, sample_time);
            assert_that(t, result, is_equal_to(Event::Generated));
          }));

    t.run("execute(l,s) reports completed if timer expires",
          test<HoldMode>([](Tester &t, Controls &controls, PhaseTimer &timer,
                            HoldMode &mode) {
            timer.advance(0.99999F);
            controls.duration_ = 1.F;
            auto constexpr sample_time = 0.1F; // Enough to advance to 1
            auto const result = mode.execute(low_latch, sample_time);
            assert_that(t, result, is_equal_to(Event::Completed));
          }));

    t.run("execute(l,s) outputs_ nothing",
          test<HoldMode>([](Tester &t, Controls &controls, PhaseTimer & /**/,
                            HoldMode &mode) {
            float original_output = 99.F;
            controls.output_ = original_output;
            mode.execute(low_latch, 0.1F);
            assert_that(t, controls.output_, is_equal_to(original_output));
          }));

    t.run("exit() deactivates stage",
          test<HoldMode>([](Tester &t, Controls &controls, PhaseTimer & /**/,
                            HoldMode &mode) {
            controls.active_ = true;
            mode.exit();
            assert_that(t, controls.active_, is_false);
          }));
  }
};

static auto _ = HoldModeSuite{};

} // namespace stage
} // namespace test
