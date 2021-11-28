#include "components/latch.h"
#include "modules/envelope/mode/events.h"
#include "modules/envelope/mode/hold-mode.h"

#include "dheunit/test.h"
#include "helpers/assertions.h"
#include "timed-mode-fixture.h"

namespace test {
namespace envelope {

using dhe::PhaseTimer;
using dhe::envelope::Event;

using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;
using test::is_false;
using test::is_true;

using HoldMode = dhe::envelope::HoldMode<Signals, PhaseTimer>;

class HoldModeSuite : public Suite {
public:
  HoldModeSuite() : Suite{"dhe::envelope::mode::Hold"} {}
  void run(Tester &t) override {
    t.run("enter() activates stage",
          test<HoldMode>([](Tester &t, Signals &signals, PhaseTimer & /**/,
                            HoldMode &mode) {
            signals.active_ = false;
            mode.enter();
            assert_that(t, signals.active_, is_true);
          }));

    t.run("enter() copies input to output",
          test<HoldMode>([](Tester &t, Signals &signals, PhaseTimer & /**/,
                            HoldMode &mode) {
            auto constexpr input = 0.234F;
            signals.input_ = input;
            mode.enter();
            assert_that(t, signals.output_, is_equal_to(input));
          }));

    t.run("enter() resets timer",
          test<HoldMode>(
              [](Tester &t, Signals & /**/, PhaseTimer &timer, HoldMode &mode) {
                timer.advance(1.F);
                mode.enter();
                assert_that(t, timer.phase(), is_equal_to(0.F));
              }));

    t.run("execute(l,s) resets timer if latch rises",
          test<HoldMode>([](Tester &t, Signals &signals, PhaseTimer &timer,
                            HoldMode &mode) {
            timer.reset();
            timer.advance(0.9F); // nearly complete before executing
            signals.duration_ = 1.F;
            auto constexpr sample_time = 0.F; // Cheat: Advance by 0 after reset
            mode.execute(dhe::latch::rising, sample_time);
            assert_that(t, timer.phase(), is_equal_to(0.F));
          }));

    t.run("execute(l,s) reports generated if timer not expired",
          test<HoldMode>([](Tester &t, Signals &signals, PhaseTimer &timer,
                            HoldMode &mode) {
            timer.reset();
            signals.duration_ = 1.F;
            auto constexpr sample_time = 0.1F;
            auto const result = mode.execute(dhe::latch::low, sample_time);
            assert_that(t, result, is_equal_to(Event::Generated));
          }));

    t.run("execute(l,s) reports completed if timer expires",
          test<HoldMode>([](Tester &t, Signals &signals, PhaseTimer &timer,
                            HoldMode &mode) {
            timer.advance(0.99999F);
            signals.duration_ = 1.F;
            auto constexpr sample_time = 0.1F; // Enough to advance to 1
            auto const result = mode.execute(dhe::latch::low, sample_time);
            assert_that(t, result, is_equal_to(Event::Completed));
          }));

    t.run("execute(l,s) outputs_ nothing",
          test<HoldMode>([](Tester &t, Signals &signals, PhaseTimer & /**/,
                            HoldMode &mode) {
            float original_output = 99.F;
            signals.output_ = original_output;
            mode.execute(dhe::latch::low, 0.1F);
            assert_that(t, signals.output_, is_equal_to(original_output));
          }));

    t.run("exit() deactivates stage",
          test<HoldMode>([](Tester &t, Signals &signals, PhaseTimer & /**/,
                            HoldMode &mode) {
            signals.active_ = true;
            mode.exit();
            assert_that(t, signals.active_, is_false);
          }));
  }
};

static auto _ = HoldModeSuite{};

} // namespace envelope
} // namespace test
