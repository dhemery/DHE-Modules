#include "modules/envelope/mode/generate.h"

#include "components/phase-timer.h"
#include "modules/envelope/mode/event.h"

#include "dheunit/test.h"
#include "helpers/assertions.h"
#include "helpers/latches.h"
#include "timed-mode-fixture.h"

namespace test {
namespace envelope {

using dhe::PhaseTimer;
using dhe::envelope::mode::Event;

using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;
using test::is_false;
using test::is_true;

using GenerateMode = dhe::envelope::mode::GenerateMode<Signals, PhaseTimer>;

class GenerateModeSuite : public Suite {
public:
  GenerateModeSuite() : Suite{"dhe::envelope::mode::Generate"} {}
  void run(Tester &t) override {
    t.run("enter() activates stage",
          test<GenerateMode>([](Tester &t, Signals &signals, PhaseTimer & /**/,
                                GenerateMode &mode) {
            signals.active_ = false;
            mode.enter();
            assert_that(t, signals.active_, is_true);
          }));

    t.run("enter() resets timer",
          test<GenerateMode>([](Tester &t, Signals & /**/, PhaseTimer &timer,
                                GenerateMode &mode) {
            timer.advance(1.F);
            mode.enter();
            assert_that(t, timer.phase(), is_equal_to(0.F));
          }));

    t.run("enter() outputs_ nothing",
          test<GenerateMode>([](Tester &t, Signals &signals, PhaseTimer & /**/,
                                GenerateMode &mode) {
            auto constexpr original_output = 7341.F;
            signals.output_ = original_output;
            mode.enter();
            assert_that(t, signals.output_, is_equal_to(original_output));
          }));

    t.run("execute(l,s) advances the timer",
          test<GenerateMode>([](Tester &t, Signals &signals, PhaseTimer &timer,
                                GenerateMode &mode) {
            mode.enter(); // resets the timer phase to 0

            auto constexpr full_duration = 3.F;
            auto constexpr phase_delta = 0.1F;
            auto constexpr sample_time = full_duration * phase_delta;

            signals.duration_ = full_duration;
            mode.execute(rising_latch, sample_time);
            assert_that(t, timer.phase(), is_equal_to(phase_delta));
          }));

    t.run("execute(l,s) outputs_ curve voltage",
          test<GenerateMode>([](Tester &t, Signals &signals, PhaseTimer &timer,
                                GenerateMode &mode) {
            auto constexpr input = 5.F;
            auto constexpr level = 10.F; // Curve from 5 to 10V
            auto constexpr curve_range = level - input;
            signals.input_ = input;
            signals.level_ = level;
            mode.enter(); // Initializes the curve to 5V at phase 0

            // Simplify calculations by using a linear taper (default J
            // curvature is 0, making it linear)
            signals.shape_ = ShapeId::J;

            auto constexpr full_duration = 10.F;
            auto constexpr phase_delta =
                0.1F; // Each step is 1/10 full duration
            auto constexpr sample_time = full_duration * phase_delta;
            signals.duration_ = full_duration;

            // Given a linear taper and a phase delta of 1/10, execute() should
            // advance the curve by 1/10 of curve range and output the result.
            auto constexpr expected_output = input + curve_range * phase_delta;

            mode.execute(low_latch, sample_time);

            assert_that(t, timer.phase(), is_equal_to(phase_delta));
            assert_that(t, signals.output_, is_equal_to(expected_output));
          }));

    t.run("execute(l,s) resets curve before generating if latch rises",
          test<GenerateMode>([](Tester &t, Signals &signals, PhaseTimer &timer,
                                GenerateMode &mode) {
            auto constexpr input_on_entry = 5.F;
            auto constexpr level = 10.F; // Full curve range on entry is 5–10V
            signals.input_ = input_on_entry;
            signals.level_ = level;
            mode.enter(); // Initializes the curve at 5V at phase 0

            // Simplify calculations by using a linear taper (default J
            // curvature is 0, making it linear)
            signals.shape_ = ShapeId::J;

            auto constexpr full_duration = 10.F;
            auto constexpr phase_delta =
                0.1F; // Each step is 1/10 full duration
            auto constexpr sample_time = full_duration * phase_delta;
            signals.duration_ = full_duration;
            mode.execute(low_latch, sample_time); // Advance to ~5.5V
            mode.execute(low_latch, sample_time); // Advance to ~6V
            mode.execute(low_latch, sample_time); // Advance to ~6.5V

            auto constexpr input_at_reset = 2.F;
            auto constexpr range_at_reset = level - input_at_reset;

            // Given a rising latch, a linear taper, and a phase delta of 1/10,
            // execute() should reset the curve to the current input voltage at
            // phase 0, advance by 1/10 of curve range, and output the result.
            auto constexpr expected_output =
                input_at_reset + range_at_reset * phase_delta;

            signals.input_ = input_at_reset;
            mode.execute(rising_latch, sample_time);

            assert_that(t, timer.phase(), is_equal_to(phase_delta));
            assert_that(t, signals.output_, is_equal_to(expected_output));
          }));

    t.run("execute(l,s) reports generated if timer not expired",
          test<GenerateMode>([](Tester &t, Signals &signals, PhaseTimer &timer,
                                GenerateMode &mode) {
            timer.reset();
            signals.duration_ = 1.F;
            auto constexpr sample_time = 0.1F;
            auto const result = mode.execute(low_latch, sample_time);
            assert_that(t, result, is_equal_to(Event::Generated));
          }));

    t.run("execute(l,s) reports completed if timer expires",
          test<GenerateMode>([](Tester &t, Signals &signals, PhaseTimer &timer,
                                GenerateMode &mode) {
            timer.advance(0.99999F);
            signals.duration_ = 1.F;
            auto constexpr sample_time = 0.1F; // Enough to advance to 1
            auto const result = mode.execute(low_latch, sample_time);
            assert_that(t, result, is_equal_to(Event::Completed));
          }));

    t.run("exit() deactivates stage",
          test<GenerateMode>([](Tester &t, Signals &signals, PhaseTimer & /**/,
                                GenerateMode &mode) {
            signals.active_ = true;
            mode.exit();
            assert_that(t, signals.active_, is_false);
          }));
  }
};

static auto _ = GenerateModeSuite{};

} // namespace envelope
} // namespace test
