#include "dheunit/test.h"
#include "helpers/assertions.h"
#include "helpers/latches.h"
#include "stage-engine-fixture.h"

namespace test {
namespace envelope {
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;
using test::is_false;
using test::is_true;

static inline void in_generate_mode(Signals &signals, SimpleMode & /**/,
                                    SimpleMode &input_mode,
                                    TimedMode &generate_mode, SimpleMode & /**/,
                                    StageEngine &engine) {
  // Starts in input mode

  // input mode + rising gate -> generate mode
  signals.gate_ = true;
  engine.process(0.F);
  signals.gate_ = false;

  // Reset the fields of the modes we've traversed
  input_mode = SimpleMode{};
  generate_mode = TimedMode{};
}

class StageEngineGenerateModeSuite : Suite {
public:
  StageEngineGenerateModeSuite()
      : Suite{"dhe::envelope::stage::Engine in generate mode"} {}
  void run(Tester &t) override {
    t.run("if defer rises: begins deferring",
          test(in_generate_mode, [](Tester &t, Signals &signals,
                                    SimpleMode &defer_mode, SimpleMode & /**/,
                                    TimedMode &generate_mode, SimpleMode & /**/,
                                    StageEngine &engine) {
            signals.defer_ = true;

            engine.process(0.F);

            assert_that(t, "exit generate", generate_mode.exited_, is_true);
            assert_that(t, "execute generate", generate_mode.executed_,
                        is_false);
            assert_that(t, "enter defer", defer_mode.entered_, is_true);
            assert_that(t, "execute defer", defer_mode.executed_, is_true);
          }));

    t.run(
        "with defer low: executes regardless of gate",
        test(in_generate_mode, [](Tester &t, Signals &signals,
                                  SimpleMode & /**/, SimpleMode & /**/,
                                  TimedMode &generate_mode, SimpleMode & /**/,
                                  StageEngine &engine) {
          signals.defer_ = false;

          signals.gate_ = true;
          engine.process(0.F);

          assert_that(t, "execute generate", generate_mode.executed_, is_true);
          assert_that(t, "exit generate", generate_mode.exited_, is_false);
          generate_mode.executed_ = false;

          signals.gate_ = true;
          engine.process(0.F);

          assert_that(t, "execute generate", generate_mode.executed_, is_true);
          assert_that(t, "exit generate", generate_mode.exited_, is_false);
        }));

    t.run(
        "passes gate state and sample time to generate.execute()",
        test(in_generate_mode, [](Tester &t, Signals &signals,
                                  SimpleMode & /**/, SimpleMode & /**/,
                                  TimedMode &generate_mode, SimpleMode & /**/,
                                  StageEngine &engine) {
          signals.defer_ = false;

          signals.gate_ = false; // Start with a step with gate low
          auto constexpr sample_time = 0.0341F;
          engine.process(sample_time);
          assert_that(t, generate_mode.sample_time_, is_equal_to(sample_time));

          signals.gate_ = false; // low + low = low latch
          engine.process(0.F);
          assert_that(t, "low", generate_mode.latch_, is_equal_to(low_latch));

          signals.gate_ = true; // low + high = rising latch
          engine.process(0.F);
          assert_that(t, "rise", generate_mode.latch_,
                      is_equal_to(rising_latch));

          signals.gate_ = true; // high + high = high latch
          engine.process(0.F);
          assert_that(t, "high", generate_mode.latch_, is_equal_to(high_latch));

          signals.gate_ = false; // high + low = falling latch
          engine.process(0.F);
          assert_that(t, "fall", generate_mode.latch_,
                      is_equal_to(falling_latch));
        }));

    t.run(
        "if generate completes: raises eoc and enters level mode",
        test(in_generate_mode, [](Tester &t, Signals &signals,
                                  SimpleMode & /**/, SimpleMode & /**/,
                                  TimedMode &generate_mode,
                                  SimpleMode &level_mode, StageEngine &engine) {
          signals.defer_ = false;
          generate_mode.event_ = Event::Completed;

          engine.process(0.F);

          assert_that(t, "raised eoc", signals.eoc_, is_true);
          assert_that(t, "execute generate", generate_mode.executed_, is_true);
          assert_that(t, "exit generate", generate_mode.exited_, is_true);
          assert_that(t, "enter level", level_mode.entered_, is_true);
          assert_that(t, "execute level", level_mode.executed_, is_false);
        }));
  }
};

static auto _ = StageEngineGenerateModeSuite{};
} // namespace envelope
} // namespace test
