#include "./fixtures/hostage-engine-fixture.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace stage {
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestRegistrar;

static inline void in_idle_mode(Controls &controls, SimpleMode &input_mode,
                                SimpleMode & /*input_mode*/,
                                TimedMode & /*hold_mode*/,
                                LatchedMode &sustain_mode,
                                SimpleMode &idle_mode, HostageEngine &engine) {
  // Starts in input mode

  // input mode + gate rise + sustain selected -> sustain mode
  controls.mode_ = Mode::Sustain;
  sustain_mode.event_ = Event::Generated;
  controls.gate_ = true;
  engine.process(0.F);

  // sustain mode + gate fall -> idle mode
  controls.gate_ = false;
  sustain_mode.event_ = Event::Completed;
  engine.process(0.F);

  // Reset the fields of the modes we've traversed
  input_mode = SimpleMode{};
  sustain_mode = LatchedMode{};
  idle_mode = SimpleMode{};
}

class HostageEngineLevelModeSuite : Suite {
public:
  HostageEngineLevelModeSuite()
      : Suite{"dhe::stage::HostageEngine in idle mode"} {}
  void register_tests(TestRegistrar add) override {
    add("if defer rises: begins deferring",
        test(in_idle_mode,
             [](Tester &t, Controls &controls, SimpleMode & /*input_mode*/,
                SimpleMode &defer_mode, TimedMode & /*hold_mode*/,
                LatchedMode & /*sustain_mode*/, SimpleMode &idle_mode,
                HostageEngine &engine) {
               controls.defer_ = true;

               engine.process(0.F);

               t.assert_that("exit idle", idle_mode.exited_, is_true);
               t.assert_that("execute idle", idle_mode.executed_, is_false);
               t.assert_that("enter defer", defer_mode.entered_, is_true);
               t.assert_that("execute defer", defer_mode.executed_, is_true);
             }));

    add("with defer low: executes if gate does not rise",
        test(in_idle_mode,
             [](Tester &t, Controls &controls, SimpleMode & /*input_mode*/,
                SimpleMode & /*defer_mode*/, TimedMode & /*hold_mode*/,
                LatchedMode & /*sustain_mode*/, SimpleMode &idle_mode,
                HostageEngine &engine) {
               controls.defer_ = false;
               controls.gate_ = false;
               engine.process(0.F);
               t.assert_that(idle_mode.executed_, is_true);
             }));

    add("with hold mode selected: if gate rises with defer low: begins holding",
        test(in_idle_mode,
             [](Tester &t, Controls &controls, SimpleMode &/*input_mode*/,
                SimpleMode & /*defer_mode*/, TimedMode &hold_mode,
                LatchedMode & /*sustain_mode*/, SimpleMode &idle_mode,
                HostageEngine &engine) {
               controls.mode_ = Mode::Hold;
               controls.defer_ = false;
               controls.gate_ = true;

               engine.process(0.F);

               t.assert_that("exit idle", idle_mode.exited_, is_true);
               t.assert_that("execute idle", idle_mode.executed_, is_false);
               t.assert_that("enter hold", hold_mode.entered_, is_true);
               t.assert_that("execute hold", hold_mode.executed_, is_true);
             }));

    add("with hold mode selected: if gate rises with defer low: begins holding",
        test(in_idle_mode,
             [](Tester &t, Controls &controls, SimpleMode &/*input_mode*/,
                SimpleMode & /*defer_mode*/, TimedMode & /*hold_mode*/,
                LatchedMode &sustain_mode, SimpleMode &idle_mode,
                HostageEngine &engine) {
               controls.mode_ = Mode::Sustain;
               controls.defer_ = false;
               controls.gate_ = true;

               engine.process(0.F);

               t.assert_that("exit idle", idle_mode.exited_, is_true);
               t.assert_that("execute idle", idle_mode.executed_, is_false);
               t.assert_that("enter sustain", sustain_mode.entered_, is_true);
               t.assert_that("execute sustain", sustain_mode.executed_,
                             is_true);
             }));
  }
};

__attribute__((unused)) static auto _ = HostageEngineLevelModeSuite{};
} // namespace stage
} // namespace test
