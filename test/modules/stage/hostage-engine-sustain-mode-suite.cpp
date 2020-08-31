#include "./fixtures/hostage-engine-fixture.h"
#include "helpers/latches.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace stage {
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestRegistrar;

static inline void in_sustain_mode(Controls &controls, SimpleMode &input_mode,
                                   SimpleMode & /*defer_mode*/,
                                   TimedMode & /*hold_mode*/,
                                   LatchedMode &sustain_mode,
                                   SimpleMode & /*idle_mode*/,
                                   HostageEngine &engine) {
  // Starts in input mode

  // input mode + gate rise + sustain selected -> sustain mode
  controls.mode_ = Mode::Sustain;
  sustain_mode.event_ = Event::Generated;
  controls.gate_ = true;
  engine.process(0.F);

  // Reset the fields of the modes we've traversed
  input_mode = SimpleMode{};
  sustain_mode = LatchedMode{};
}

class HostageEngineSustainModeSuite : Suite {
public:
  HostageEngineSustainModeSuite()
      : Suite{"dhe::stage::HostageEngine in sustain mode"} {}
  void register_tests(TestRegistrar add) override {
    add("if defer rises: begins deferring",
        test(in_sustain_mode,
             [](Tester &t, Controls &controls, SimpleMode & /*input_mode*/,
                SimpleMode &defer_mode, TimedMode & /*hold_mode*/,
                LatchedMode &sustain_mode, SimpleMode & /*idle_mode*/,
                HostageEngine &engine) {
               controls.defer_ = true;

               engine.process(0.F);

               t.assert_that("exit sustain", sustain_mode.exited_, is_true);
               t.assert_that("execute sustain", sustain_mode.executed_,
                             is_false);
               t.assert_that("enter defer", defer_mode.entered_, is_true);
               t.assert_that("execute defer", defer_mode.executed_, is_true);
             }));

    add("with defer low: executes regardless of gate",
        test(
            in_sustain_mode,
            [](Tester &t, Controls &controls, SimpleMode & /*input_mode*/,
               SimpleMode &defer_mode, TimedMode & /*hold_mode*/,
               LatchedMode &sustain_mode, SimpleMode & /*idle_mode*/,
               HostageEngine &engine) {
              controls.defer_ = false;

              controls.gate_ = true;
              engine.process(0.F);

              t.assert_that("execute sustain", sustain_mode.executed_, is_true);
              t.assert_that("exit sustain", sustain_mode.exited_, is_false);
              sustain_mode.executed_ = false;

              controls.gate_ = true;
              engine.process(0.F);

              t.assert_that("execute sustain", sustain_mode.executed_, is_true);
              t.assert_that("exit sustain", sustain_mode.exited_, is_false);
            }));

    add("passes gate state to sustain.execute()",
        test(in_sustain_mode, [](Tester &t, Controls &controls,
                                 SimpleMode & /*input_mode*/,
                                 SimpleMode &defer_mode,
                                 TimedMode & /*hold_mode*/,
                                 LatchedMode &sustain_mode,
                                 SimpleMode & /*idle_mode*/,
                                 HostageEngine &engine) {
          controls.defer_ = false;

          controls.gate_ = false; // Start with a step with gate low
          engine.process(0.F);

          controls.gate_ = false; // low + low = low latch
          engine.process(0.F);
          t.assert_that("low", sustain_mode.latch_, is_equal_to(low_latch));

          controls.gate_ = true; // low + high = rising latch
          engine.process(0.F);
          t.assert_that("rise", sustain_mode.latch_, is_equal_to(rising_latch));

          controls.gate_ = true; // high + high = high latch
          engine.process(0.F);
          t.assert_that("high", sustain_mode.latch_, is_equal_to(high_latch));

          controls.gate_ = false; // high + low = falling latch
          engine.process(0.F);
          t.assert_that("fall", sustain_mode.latch_,
                        is_equal_to(falling_latch));
        }));

    add("if sustain completes: raises eoc and enters idle mode",
        test(in_sustain_mode,
             [](Tester &t, Controls &controls, SimpleMode & /*input_mode*/,
                SimpleMode & /*defer_mode*/, TimedMode & /*hold_mode*/,
                LatchedMode &sustain_mode, SimpleMode &idle_mode,
                HostageEngine &engine) {
               controls.defer_ = false;
               sustain_mode.event_ = Event::Completed;

               engine.process(0.F);

               t.assert_that("raised eoc", controls.eoc_, is_true);
               t.assert_that("execute sustain", sustain_mode.executed_,
                             is_true);
               t.assert_that("exit sustain", sustain_mode.exited_, is_true);
               t.assert_that("enter level", idle_mode.entered_, is_true);
               t.assert_that("execute level", idle_mode.executed_, is_false);
             }));
  }
};
__attribute__((unused)) static auto _ = HostageEngineSustainModeSuite{};
} // namespace stage
} // namespace test
