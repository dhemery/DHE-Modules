#include "dheunit/test.h"
#include "helpers/assertions.h"
#include "hostage-engine-fixture.h"

namespace test {
namespace hostage {
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_false;
using test::is_true;

static inline void in_input_mode(Signals & /*signals*/, SimpleMode &input_mode,
                                 SimpleMode & /*defer_mode*/,
                                 TimedMode & /*hold_mode*/,
                                 LatchedMode & /*sustain_mode*/,
                                 SimpleMode & /*idle_mode*/,
                                 HostageEngine &engine) {
  // Starts in input mode, but let's run one sample anyway
  engine.process(0.F);
  input_mode = SimpleMode{};
}

class HostageEngineInputModeSuite : Suite {
public:
  HostageEngineInputModeSuite()
      : Suite{"dhe::envelope::hostage::Engine in input mode"} {}
  void run(Tester &t) override {
    t.run("if defer rises: begins deferring",
          test(in_input_mode, [](Tester &t, Signals &signals,
                                 SimpleMode &input_mode, SimpleMode &defer_mode,
                                 TimedMode & /*hold_mode*/,
                                 LatchedMode & /*sustain_mode*/,
                                 SimpleMode & /**/, HostageEngine &engine) {
            signals.defer_ = true;

            engine.process(0.F);

            assert_that(t, "exit input", input_mode.exited_, is_true);
            assert_that(t, "execute input", input_mode.executed_, is_false);
            assert_that(t, "enter defer", defer_mode.entered_, is_true);
            assert_that(t, "execute defer", defer_mode.executed_, is_true);
          }));

    t.run(
        "with hold mode selected: if gate rises with defer low: begins holding",
        test(in_input_mode,
             [](Tester &t, Signals &signals, SimpleMode &input_mode,
                SimpleMode & /*defer_mode*/, TimedMode &hold_mode,
                LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                HostageEngine &engine) {
               signals.mode_ = ModeId::Hold;
               signals.defer_ = false;
               signals.gate_ = true;

               engine.process(0.F);

               assert_that(t, "exit input", input_mode.exited_, is_true);
               assert_that(t, "execute input", input_mode.executed_, is_false);
               assert_that(t, "enter hold", hold_mode.entered_, is_true);
               assert_that(t, "execute hold", hold_mode.executed_, is_true);
             }));

    t.run(
        "with sustain mode selected: if gate rises with defer low: begins "
        "sustaining",
        test(in_input_mode,
             [](Tester &t, Signals &signals, SimpleMode &input_mode,
                SimpleMode & /*defer_mode*/, TimedMode & /*hold_mode*/,
                LatchedMode &sustain_mode, SimpleMode & /*idle_mode*/,
                HostageEngine &engine) {
               signals.mode_ = ModeId::Sustain;
               signals.defer_ = false;
               signals.gate_ = true;

               engine.process(0.F);

               assert_that(t, "exit input", input_mode.exited_, is_true);
               assert_that(t, "execute input", input_mode.executed_, is_false);
               assert_that(t, "enter sustain", sustain_mode.entered_, is_true);
               assert_that(t, "execute sustain", sustain_mode.executed_,
                           is_true);
             }));

    t.run("with defer low: executes if gate does not rise",
          test(in_input_mode,
               [](Tester &t, Signals &signals, SimpleMode &input_mode,
                  SimpleMode & /*defer_mode*/, TimedMode & /*hold_mode*/,
                  LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                  HostageEngine &engine) {
                 signals.defer_ = false;
                 signals.gate_ = false;
                 engine.process(0.F);
                 assert_that(t, input_mode.executed_, is_true);
               }));
  }
};

static auto _ = HostageEngineInputModeSuite{};
} // namespace hostage
} // namespace test
