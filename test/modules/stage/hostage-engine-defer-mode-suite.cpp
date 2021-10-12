#include "./fixtures/hostage-engine-fixture.h"
#include "helpers/assertions.h"

#include <dheunit/test.h>

namespace test {
namespace stage {

using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_false;
using test::is_true;

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
  void run(Tester &t) override {
    t.run("with defer high: executes regardless of gate",
          test(in_defer_mode,
               [](Tester &t, Controls &controls, SimpleMode & /*input_mode*/,
                  SimpleMode &defer_mode, TimedMode & /*hold_mode*/,
                  LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                  HostageEngine &engine) {
                 controls.defer_ = true;

                 controls.gate_ = true;
                 engine.process(0.F);
                 assert_that(t, "with gate high", defer_mode.executed_,
                             is_true);

                 defer_mode.executed_ = false;
                 controls.gate_ = false;
                 engine.process(0.F);
                 assert_that(t, "with gate low", defer_mode.executed_, is_true);
               }));

    t.run("with hold mode selected: if defer falls with gate high: begins "
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

                 assert_that(t, "exit defer", defer_mode.exited_, is_true);
                 assert_that(t, "execute defer", defer_mode.executed_,
                             is_false);
                 assert_that(t, "enter hold", hold_mode.entered_, is_true);
                 assert_that(t, "execute hold", hold_mode.executed_, is_true);
               }));

    t.run("with hold mode selected: if defer falls with gate low: begins "
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

                 assert_that(t, "exit defer", defer_mode.exited_, is_true);
                 assert_that(t, "execute defer", defer_mode.executed_,
                             is_false);
                 assert_that(t, "enter input", input_mode.entered_, is_true);
                 assert_that(t, "execute input", input_mode.executed_, is_true);
                 assert_that(t, "raised eoc", controls.eoc_, is_false);
               }));

    t.run("with sustain mode selected: if defer falls with gate high: begins "
          "sustaining",
          test(in_defer_mode, [](Tester &t, Controls &controls,
                                 SimpleMode & /*input_mode*/,
                                 SimpleMode &defer_mode, TimedMode &hold_mode,
                                 LatchedMode &sustain_mode,
                                 SimpleMode & /*idle_mode*/,
                                 HostageEngine &engine) {
            controls.mode_ = Mode::Sustain;
            controls.defer_ = false;
            controls.gate_ = true;

            engine.process(0.F);

            assert_that(t, "exit defer", defer_mode.exited_, is_true);
            assert_that(t, "execute defer", defer_mode.executed_, is_false);
            assert_that(t, "enter sustain", sustain_mode.entered_, is_true);
            assert_that(t, "execute sustain", sustain_mode.executed_, is_true);
          }));

    t.run(
        "with sustain mode selected: if defer falls with gate low: raises eoc "
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

               assert_that(t, "exit defer", defer_mode.exited_, is_true);
               assert_that(t, "execute defer", defer_mode.executed_, is_false);
               assert_that(t, "enter idle", idle_mode.entered_, is_true);
               assert_that(t, "execute idle", idle_mode.executed_, is_true);
               assert_that(t, "raised eoc", controls.eoc_, is_true);
             }));

    t.run("if defer falls with gate low: begins tracking input without raising "
          "eoc",
          test(in_defer_mode,
               [](Tester &t, Controls &controls, SimpleMode &input_mode,
                  SimpleMode &defer_mode, TimedMode & /*hold_mode*/,
                  LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                  HostageEngine &engine) {
                 controls.defer_ = false;
                 controls.gate_ = false;

                 engine.process(0.F);

                 assert_that(t, "exit defer", defer_mode.exited_, is_true);
                 assert_that(t, "execute defer", defer_mode.executed_,
                             is_false);
                 assert_that(t, "enter input", input_mode.entered_, is_true);
                 assert_that(t, "execute input", input_mode.executed_, is_true);
                 assert_that(t, "raised eoc", controls.eoc_, is_false);
               }));
  }
};

static auto _ = HostageEngineDeferModeSuite{};
} // namespace stage
} // namespace test
