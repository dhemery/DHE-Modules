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

static inline void in_defer_mode(Controls &controls, SimpleMode &input_mode,
                                 SimpleMode &defer_mode,
                                 TimedMode & /*hold_mode*/,
                                 LatchedMode & /*sustain_mode*/,
                                 SimpleMode & /*idle_mode*/,
                                 HostageEngine &engine) {
  // Starts in input mode

  // input mode + defer rise -> defer mode
  controls.defer_ = true;
  engine.process(0.F);
  // leave defer high

  // Reset the fields of the modes we've traversed
  input_mode = SimpleMode{};
  defer_mode = SimpleMode{};
}

class HostageEngineDeferModeSuite : Suite {
public:
  HostageEngineDeferModeSuite()
      : Suite{"dhe::stage::HostageEngine in defer mode"} {}
  void register_tests(TestRegistrar add) override {
    add("with defer high: executes regardless of gate",
        test(in_defer_mode,
             [](Tester &t, Controls &controls, SimpleMode & /*input_mode*/,
                SimpleMode &defer_mode, TimedMode & /*hold_mode*/,
                LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                HostageEngine &engine) {
               controls.defer_ = true;

               controls.gate_ = true;
               engine.process(0.F);
               t.assert_that("with gate high", defer_mode.executed_, is_true);

               defer_mode.executed_ = false;
               controls.gate_ = false;
               engine.process(0.F);
               t.assert_that("with gate low", defer_mode.executed_, is_true);
             }));

    add("with hold mode selected: if defer falls with gate high: begins "
        "holding",
        test(in_defer_mode,
             [](Tester &t, Controls &controls, SimpleMode & /*input_mode*/,
                SimpleMode &defer_mode, TimedMode &hold_mode,
                LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                HostageEngine &engine) {
               controls.mode_ = Mode::Hold;
               controls.defer_ = false;
               controls.gate_ = true;

               engine.process(0.F);

               t.assert_that("exit defer", defer_mode.exited_, is_true);
               t.assert_that("execute defer", defer_mode.executed_, is_false);
               t.assert_that("enter hold", hold_mode.entered_, is_true);
               t.assert_that("execute hold", hold_mode.executed_, is_true);
             }));

    add("with hold mode selected: if defer falls with gate low: begins "
        "tracking input without raising eoc",
        test(in_defer_mode,
             [](Tester &t, Controls &controls, SimpleMode &input_mode,
                SimpleMode &defer_mode, TimedMode & /*hold_mode*/,
                LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                HostageEngine &engine) {
               controls.mode_ = Mode::Hold;
               controls.defer_ = false;
               controls.gate_ = false;

               engine.process(0.F);

               t.assert_that("exit defer", defer_mode.exited_, is_true);
               t.assert_that("execute defer", defer_mode.executed_, is_false);
               t.assert_that("enter input", input_mode.entered_, is_true);
               t.assert_that("execute input", input_mode.executed_, is_true);
               t.assert_that("raised eoc", controls.eoc_, is_false);
             }));

    add("with sustain mode selected: if defer falls with gate high: begins "
        "sustaining",
        test(in_defer_mode,
             [](Tester &t, Controls &controls, SimpleMode & /*input_mode*/,
                SimpleMode &defer_mode, TimedMode &hold_mode,
                LatchedMode &sustain_mode, SimpleMode & /*idle_mode*/,
                HostageEngine &engine) {
               controls.mode_ = Mode::Sustain;
               controls.defer_ = false;
               controls.gate_ = true;

               engine.process(0.F);

               t.assert_that("exit defer", defer_mode.exited_, is_true);
               t.assert_that("execute defer", defer_mode.executed_, is_false);
               t.assert_that("enter sustain", sustain_mode.entered_, is_true);
               t.assert_that("execute sustain", sustain_mode.executed_,
                             is_true);
             }));

    add("with sustain mode selected: if defer falls with gate low: raises eoc "
        "and becomes idle",
        test(in_defer_mode,
             [](Tester &t, Controls &controls, SimpleMode & /*input_mode*/,
                SimpleMode &defer_mode, TimedMode & /*hold_mode*/,
                LatchedMode & /*sustain_mode*/, SimpleMode &idle_mode,
                HostageEngine &engine) {
               controls.mode_ = Mode::Sustain;
               controls.defer_ = false;
               controls.gate_ = false;

               engine.process(0.F);

               t.assert_that("exit defer", defer_mode.exited_, is_true);
               t.assert_that("execute defer", defer_mode.executed_, is_false);
               t.assert_that("enter idle", idle_mode.entered_, is_true);
               t.assert_that("execute idle", idle_mode.executed_, is_true);
               t.assert_that("raised eoc", controls.eoc_, is_true);
             }));

    add("if defer falls with gate low: begins tracking input without raising "
        "eoc",
        test(in_defer_mode,
             [](Tester &t, Controls &controls, SimpleMode &input_mode,
                SimpleMode &defer_mode, TimedMode & /*hold_mode*/,
                LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                HostageEngine &engine) {
               controls.defer_ = false;
               controls.gate_ = false;

               engine.process(0.F);

               t.assert_that("exit defer", defer_mode.exited_, is_true);
               t.assert_that("execute defer", defer_mode.executed_, is_false);
               t.assert_that("enter input", input_mode.entered_, is_true);
               t.assert_that("execute input", input_mode.executed_, is_true);
               t.assert_that("raised eoc", controls.eoc_, is_false);
             }));
  }
};
__attribute__((unused)) static auto _ = HostageEngineDeferModeSuite{};
} // namespace stage
} // namespace test
