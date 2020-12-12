#include "./fixtures/timed-mode-fixture.h"
#include "components/phase-timer.h"
#include "helpers/latches.h"
#include "modules/stage/event.h"
#include "modules/stage/generate-mode.h"

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

using GenerateMode = dhe::stage::GenerateMode<Controls, PhaseTimer>;

class GenerateModeSuite : public Suite {
public:
  GenerateModeSuite() : Suite{"dhe::stage::GenerateMode"} {}
  void run(Tester &t) {
    t.run("enter() activates stage",
          test<GenerateMode>([](Tester &t, Controls &controls,
                                PhaseTimer & /**/, GenerateMode &mode) {
            controls.active_ = false;
            mode.enter();
            t.assert_that(controls.active_, is_true);
          }));

    t.run("enter() resets timer",
          test<GenerateMode>([](Tester &t, Controls & /**/, PhaseTimer &timer,
                                GenerateMode &mode) {
            timer.advance(1.F);
            mode.enter();
            t.assert_that(timer.phase(), is_equal_to(0.F));
          }));

    t.run("enter() outputs nothing",
          test<GenerateMode>([](Tester &t, Controls &controls,
                                PhaseTimer & /**/, GenerateMode &mode) {
            auto constexpr original_output = 7341.F;
            controls.output_ = original_output;
            mode.enter();
            t.assert_that(controls.output_, is_equal_to(original_output));
          }));

    t.run("execute(l,s) advances the timer",
          test<GenerateMode>([](Tester &t, Controls &controls,
                                PhaseTimer &timer, GenerateMode &mode) {
            mode.enter(); // resets the timer phase to 0

            auto constexpr full_duration = 3.F;
            auto constexpr phase_delta = 0.1F;
            auto constexpr sample_time = full_duration * phase_delta;

            controls.duration_ = full_duration;
            mode.execute(rising_latch, sample_time);
            t.assert_that(timer.phase(), is_equal_to(phase_delta));
          }));

    t.run("execute(l,s) outputs curve voltage",
          test<GenerateMode>([](Tester &t, Controls &controls,
                                PhaseTimer &timer, GenerateMode &mode) {
            auto constexpr input = 5.F;
            auto constexpr level = 10.F; // Curve from 5 to 10V
            auto constexpr curve_range = level - input;
            controls.input_ = input;
            controls.level_ = level;
            mode.enter(); // Initializes the curve to 5V at phase 0

            // Simplify calculations by using a linear taper (default j_taper
            // curvature is 0, making it linear)
            controls.taper_ = dhe::sigmoid::j_taper;

            auto constexpr full_duration = 10.F;
            auto constexpr phase_delta =
                0.1F; // Each step is 1/10 full duration
            auto constexpr sample_time = full_duration * phase_delta;
            controls.duration_ = full_duration;

            // Given a linear taper and a phase delta of 1/10, execute() should
            // advance the curve by 1/10 of curve range and output the result.
            auto constexpr expected_output = input + curve_range * phase_delta;

            mode.execute(low_latch, sample_time);

            t.assert_that(timer.phase(), is_equal_to(phase_delta));
            t.assert_that(controls.output_, is_equal_to(expected_output));
          }));

    t.run("execute(l,s) resets curve before generating if latch rises",
          test<GenerateMode>([](Tester &t, Controls &controls,
                                PhaseTimer &timer, GenerateMode &mode) {
            auto constexpr input_on_entry = 5.F;
            auto constexpr level = 10.F; // Full curve range on entry is 5–10V
            controls.input_ = input_on_entry;
            controls.level_ = level;
            mode.enter(); // Initializes the curve at 5V at phase 0

            // Simplify calculations by using a linear taper (default j_taper
            // curvature is 0, making it linear)
            controls.taper_ = dhe::sigmoid::j_taper;

            auto constexpr full_duration = 10.F;
            auto constexpr phase_delta =
                0.1F; // Each step is 1/10 full duration
            auto constexpr sample_time = full_duration * phase_delta;
            controls.duration_ = full_duration;
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

            controls.input_ = input_at_reset;
            mode.execute(rising_latch, sample_time);

            t.assert_that(timer.phase(), is_equal_to(phase_delta));
            t.assert_that(controls.output_, is_equal_to(expected_output));
          }));

    t.run("execute(l,s) reports generated if timer not expired",
          test<GenerateMode>([](Tester &t, Controls &controls,
                                PhaseTimer &timer, GenerateMode &mode) {
            timer.reset();
            controls.duration_ = 1.F;
            auto constexpr sample_time = 0.1F;
            auto const result = mode.execute(low_latch, sample_time);
            t.assert_that(result, is_equal_to(Event::Generated));
          }));

    t.run("execute(l,s) reports completed if timer expires",
          test<GenerateMode>([](Tester &t, Controls &controls,
                                PhaseTimer &timer, GenerateMode &mode) {
            timer.advance(0.99999F);
            controls.duration_ = 1.F;
            auto constexpr sample_time = 0.1F; // Enough to advance to 1
            auto const result = mode.execute(low_latch, sample_time);
            t.assert_that(result, is_equal_to(Event::Completed));
          }));

    t.run("exit() deactivates stage",
          test<GenerateMode>([](Tester &t, Controls &controls,
                                PhaseTimer & /**/, GenerateMode &mode) {
            controls.active_ = true;
            mode.exit();
            t.assert_that(controls.active_, is_false);
          }));
  }
};

__attribute__((unused)) static auto _ = GenerateModeSuite{};

} // namespace stage
}; // namespace test
