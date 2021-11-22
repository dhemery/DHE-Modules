#include "components/latch.h"
#include "hostage-engine-fixture.h"

#include "dheunit/test.h"
#include "helpers/assertions.h"

namespace test {
namespace hostage {
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;
using test::is_false;
using test::is_true;
static auto constexpr low_latch = dhe::latch::low;
static auto constexpr high_latch = dhe::latch::high;
static auto constexpr falling_latch = dhe::latch::falling;
static auto constexpr rising_latch = dhe::latch::rising;

static inline void in_hold_mode(Signals &signals, SimpleMode &input_mode,
                                SimpleMode & /*defer_mode*/,
                                TimedMode &hold_mode,
                                LatchedMode & /*sustain_mode*/,
                                SimpleMode & /*idle_mode*/,
                                HostageEngine &engine) {
  // Starts in input mode

  // input mode + gate rise + hold selected -> hold mode
  signals.mode_ = Mode::Hold;
  hold_mode.event_ = Event::Generated;
  signals.gate_ = true;
  engine.process(0.F);

  // Reset the fields of the modes we've traversed
  input_mode = SimpleMode{};
  hold_mode = TimedMode{};
}

class HostageEngineHoldModeSuite : Suite {
public:
  HostageEngineHoldModeSuite()
      : Suite{"dhe::envelope::hostage::Engine in hold mode"} {}
  void run(Tester &t) override {
    t.run("if defer rises: begins deferring",
          test(in_hold_mode,
               [](Tester &t, Signals &signals, SimpleMode & /*input_mode*/,
                  SimpleMode &defer_mode, TimedMode &hold_mode,
                  LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                  HostageEngine &engine) {
                 signals.defer_ = true;

                 engine.process(0.F);

                 assert_that(t, "exit hold", hold_mode.exited_, is_true);
                 assert_that(t, "execute hold", hold_mode.executed_, is_false);
                 assert_that(t, "enter defer", defer_mode.entered_, is_true);
                 assert_that(t, "execute defer", defer_mode.executed_, is_true);
               }));

    t.run("with defer low: executes regardless of gate",
          test(in_hold_mode,
               [](Tester &t, Signals &signals, SimpleMode & /*input_mode*/,
                  SimpleMode &defer_mode, TimedMode &hold_mode,
                  LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                  HostageEngine &engine) {
                 signals.defer_ = false;

                 signals.gate_ = true;
                 engine.process(0.F);

                 assert_that(t, "execute hold", hold_mode.executed_, is_true);
                 assert_that(t, "exit hold", hold_mode.exited_, is_false);
                 hold_mode.executed_ = false;

                 signals.gate_ = true;
                 engine.process(0.F);

                 assert_that(t, "execute hold", hold_mode.executed_, is_true);
                 assert_that(t, "exit hold", hold_mode.exited_, is_false);
               }));

    t.run("passes gate state and sample time to hold.execute()",
          test(in_hold_mode, [](Tester &t, Signals &signals,
                                SimpleMode & /*input_mode*/,
                                SimpleMode &defer_mode, TimedMode &hold_mode,
                                LatchedMode & /*sustain_mode*/,
                                SimpleMode & /*idle_mode*/,
                                HostageEngine &engine) {
            signals.defer_ = false;

            signals.gate_ = false; // Start with a step with gate low
            auto constexpr sample_time = 0.0341F;
            engine.process(sample_time);
            assert_that(t, hold_mode.sample_time_, is_equal_to(sample_time));

            signals.gate_ = false; // low + low = low latch
            engine.process(0.F);
            assert_that(t, "low", hold_mode.latch_, is_equal_to(low_latch));

            signals.gate_ = true; // low + high = rising latch
            engine.process(0.F);
            assert_that(t, "rise", hold_mode.latch_, is_equal_to(rising_latch));

            signals.gate_ = true; // high + high = high latch
            engine.process(0.F);
            assert_that(t, "high", hold_mode.latch_, is_equal_to(high_latch));

            signals.gate_ = false; // high + low = falling latch
            engine.process(0.F);
            assert_that(t, "fall", hold_mode.latch_,
                        is_equal_to(falling_latch));
          }));

    t.run("if hold completes: raises eoc and enters idle mode",
          test(in_hold_mode,
               [](Tester &t, Signals &signals, SimpleMode & /*input_mode*/,
                  SimpleMode & /*defer_mode*/, TimedMode &hold_mode,
                  LatchedMode & /*sustain_mode*/, SimpleMode &idle_mode,
                  HostageEngine &engine) {
                 signals.defer_ = false;
                 hold_mode.event_ = Event::Completed;

                 engine.process(0.F);

                 assert_that(t, "raised eoc", signals.eoc_, is_true);
                 assert_that(t, "execute hold", hold_mode.executed_, is_true);
                 assert_that(t, "exit hold", hold_mode.exited_, is_true);
                 assert_that(t, "enter level", idle_mode.entered_, is_true);
                 assert_that(t, "execute level", idle_mode.executed_, is_false);
               }));
  }
};

static auto _ = HostageEngineHoldModeSuite{};
} // namespace hostage
} // namespace test
