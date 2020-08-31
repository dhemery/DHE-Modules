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

static inline void in_hold_mode(Controls &controls, SimpleMode &input_mode,
                                SimpleMode & /*defer_mode*/,
                                TimedMode &hold_mode,
                                LatchedMode & /*sustain_mode*/,
                                SimpleMode & /*idle_mode*/,
                                HostageEngine &engine) {
  // Starts in input mode

  // input mode + gate rise + hold selected -> hold mode
  controls.mode_ = Mode::Hold;
  hold_mode.event_ = Event::Generated;
  controls.gate_ = true;
  engine.process(0.F);

  // Reset the fields of the modes we've traversed
  input_mode = SimpleMode{};
  hold_mode = TimedMode{};
}

class HostageEngineGenerateModeSuite : Suite {
public:
  HostageEngineGenerateModeSuite()
      : Suite{"dhe::stage::HostageEngine in hold mode"} {}
  void register_tests(TestRegistrar add) override {
    add("if defer rises: begins deferring",
        test(in_hold_mode,
             [](Tester &t, Controls &controls, SimpleMode & /*input_mode*/,
                SimpleMode &defer_mode, TimedMode &hold_mode,
                LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                HostageEngine &engine) {
               controls.defer_ = true;

               engine.process(0.F);

               t.assert_that("exit hold", hold_mode.exited_, is_true);
               t.assert_that("execute hold", hold_mode.executed_, is_false);
               t.assert_that("enter defer", defer_mode.entered_, is_true);
               t.assert_that("execute defer", defer_mode.executed_, is_true);
             }));

    add("with defer low: executes regardless of gate",
        test(in_hold_mode,
             [](Tester &t, Controls &controls, SimpleMode & /*input_mode*/,
                SimpleMode &defer_mode, TimedMode &hold_mode,
                LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                HostageEngine &engine) {
               controls.defer_ = false;

               controls.gate_ = true;
               engine.process(0.F);

               t.assert_that("execute hold", hold_mode.executed_, is_true);
               t.assert_that("exit hold", hold_mode.exited_, is_false);
               hold_mode.executed_ = false;

               controls.gate_ = true;
               engine.process(0.F);

               t.assert_that("execute hold", hold_mode.executed_, is_true);
               t.assert_that("exit hold", hold_mode.exited_, is_false);
             }));

    add("passes gate state and sample time to hold.execute()",
        test(in_hold_mode, [](Tester &t, Controls &controls,
                              SimpleMode & /*input_mode*/,
                              SimpleMode &defer_mode, TimedMode &hold_mode,
                              LatchedMode & /*sustain_mode*/,
                              SimpleMode & /*idle_mode*/,
                              HostageEngine &engine) {
          controls.defer_ = false;

          controls.gate_ = false; // Start with a step with gate low
          auto constexpr sample_time = 0.0341F;
          engine.process(sample_time);
          t.assert_that(hold_mode.sample_time_, is_equal_to(sample_time));

          controls.gate_ = false; // low + low = low latch
          engine.process(0.F);
          t.assert_that("low", hold_mode.latch_, is_equal_to(low_latch));

          controls.gate_ = true; // low + high = rising latch
          engine.process(0.F);
          t.assert_that("rise", hold_mode.latch_, is_equal_to(rising_latch));

          controls.gate_ = true; // high + high = high latch
          engine.process(0.F);
          t.assert_that("high", hold_mode.latch_, is_equal_to(high_latch));

          controls.gate_ = false; // high + low = falling latch
          engine.process(0.F);
          t.assert_that("fall", hold_mode.latch_, is_equal_to(falling_latch));
        }));

    add("if hold completes: raises eoc and enters idle mode",
        test(in_hold_mode,
             [](Tester &t, Controls &controls, SimpleMode & /*input_mode*/,
                SimpleMode & /*defer_mode*/, TimedMode &hold_mode,
                LatchedMode & /*sustain_mode*/, SimpleMode &idle_mode,
                HostageEngine &engine) {
               controls.defer_ = false;
               hold_mode.event_ = Event::Completed;

               engine.process(0.F);

               t.assert_that("raised eoc", controls.eoc_, is_true);
               t.assert_that("execute hold", hold_mode.executed_, is_true);
               t.assert_that("exit hold", hold_mode.exited_, is_true);
               t.assert_that("enter level", idle_mode.entered_, is_true);
               t.assert_that("execute level", idle_mode.executed_, is_false);
             }));
  }
};
__attribute__((unused)) static auto _ = HostageEngineGenerateModeSuite{};
} // namespace stage
} // namespace test
