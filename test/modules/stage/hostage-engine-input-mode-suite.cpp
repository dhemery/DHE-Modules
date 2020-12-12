#include "./fixtures/hostage-engine-fixture.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace stage {
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;

static inline void
in_input_mode(Controls & /*controls*/, SimpleMode &input_mode,
              SimpleMode & /*defer_mode*/, TimedMode & /*hold_mode*/,
              LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
              HostageEngine &engine) {
  // Starts in input mode, but let's run one sample anyway
  engine.process(0.F);
  input_mode = SimpleMode{};
}

class HostageEngineInputModeSuite : Suite {
public:
  HostageEngineInputModeSuite()
      : Suite{"dhe::stage::HostageEngine in input mode"} {}
  void run(Tester &t) override {
    t.run("if defer rises: begins deferring",
          test(in_input_mode,
               [](Tester &t, Controls &controls, SimpleMode &input_mode,
                  SimpleMode &defer_mode, TimedMode & /*hold_mode*/,
                  LatchedMode & /*sustain_mode*/, SimpleMode & /**/,
                  HostageEngine &engine) {
                 controls.defer_ = true;

                 engine.process(0.F);

                 t.assert_that("exit input", input_mode.exited_, is_true);
                 t.assert_that("execute input", input_mode.executed_, is_false);
                 t.assert_that("enter defer", defer_mode.entered_, is_true);
                 t.assert_that("execute defer", defer_mode.executed_, is_true);
               }));

    t.run(
        "with hold mode selected: if gate rises with defer low: begins holding",
        test(in_input_mode,
             [](Tester &t, Controls &controls, SimpleMode &input_mode,
                SimpleMode & /*defer_mode*/, TimedMode &hold_mode,
                LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                HostageEngine &engine) {
               controls.mode_ = Mode::Hold;
               controls.defer_ = false;
               controls.gate_ = true;

               engine.process(0.F);

               t.assert_that("exit input", input_mode.exited_, is_true);
               t.assert_that("execute input", input_mode.executed_, is_false);
               t.assert_that("enter hold", hold_mode.entered_, is_true);
               t.assert_that("execute hold", hold_mode.executed_, is_true);
             }));

    t.run("with sustain mode selected: if gate rises with defer low: begins "
          "sustaining",
          test(in_input_mode,
               [](Tester &t, Controls &controls, SimpleMode &input_mode,
                  SimpleMode & /*defer_mode*/, TimedMode & /*hold_mode*/,
                  LatchedMode &sustain_mode, SimpleMode & /*idle_mode*/,
                  HostageEngine &engine) {
                 controls.mode_ = Mode::Sustain;
                 controls.defer_ = false;
                 controls.gate_ = true;

                 engine.process(0.F);

                 t.assert_that("exit input", input_mode.exited_, is_true);
                 t.assert_that("execute input", input_mode.executed_, is_false);
                 t.assert_that("enter sustain", sustain_mode.entered_, is_true);
                 t.assert_that("execute sustain", sustain_mode.executed_,
                               is_true);
               }));

    t.run("with defer low: executes if gate does not rise",
          test(in_input_mode,
               [](Tester &t, Controls &controls, SimpleMode &input_mode,
                  SimpleMode & /*defer_mode*/, TimedMode & /*hold_mode*/,
                  LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                  HostageEngine &engine) {
                 controls.defer_ = false;
                 controls.gate_ = false;
                 engine.process(0.F);
                 t.assert_that(input_mode.executed_, is_true);
               }));
  }
};
__attribute__((unused)) static auto _ = HostageEngineInputModeSuite{};

} // namespace stage
} // namespace test
