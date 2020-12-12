#include "./fixtures/stage-engine-fixture.h"
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

static inline void in_generate_mode(Controls &controls, SimpleMode & /**/,
                                    SimpleMode &input_mode,
                                    TimedMode &generate_mode, SimpleMode & /**/,
                                    StageEngine &engine) {
  // Starts in input mode

  // input mode + rising gate -> generate mode
  controls.gate_ = true;
  engine.process(0.F);
  controls.gate_ = false;

  // Reset the fields of the modes we've traversed
  input_mode = SimpleMode{};
  generate_mode = TimedMode{};
}

class StageEngineGenerateModeSuite : Suite {
public:
  StageEngineGenerateModeSuite()
      : Suite{"dhe::stage::StageEngine in generate mode"} {}
  void run(Tester &t) override {
    t.run("if defer rises: begins deferring",
          test(in_generate_mode,
               [](Tester &t, Controls &controls, SimpleMode &defer_mode,
                  SimpleMode & /**/, TimedMode &generate_mode,
                  SimpleMode & /**/, StageEngine &engine) {
                 controls.defer_ = true;

                 engine.process(0.F);

                 t.assert_that("exit generate", generate_mode.exited_, is_true);
                 t.assert_that("execute generate", generate_mode.executed_,
                               is_false);
                 t.assert_that("enter defer", defer_mode.entered_, is_true);
                 t.assert_that("execute defer", defer_mode.executed_, is_true);
               }));

    t.run("with defer low: executes regardless of gate",
          test(in_generate_mode, [](Tester &t, Controls &controls,
                                    SimpleMode & /**/, SimpleMode & /**/,
                                    TimedMode &generate_mode, SimpleMode & /**/,
                                    StageEngine &engine) {
            controls.defer_ = false;

            controls.gate_ = true;
            engine.process(0.F);

            t.assert_that("execute generate", generate_mode.executed_, is_true);
            t.assert_that("exit generate", generate_mode.exited_, is_false);
            generate_mode.executed_ = false;

            controls.gate_ = true;
            engine.process(0.F);

            t.assert_that("execute generate", generate_mode.executed_, is_true);
            t.assert_that("exit generate", generate_mode.exited_, is_false);
          }));

    t.run("passes gate state and sample time to generate.execute()",
          test(in_generate_mode, [](Tester &t, Controls &controls,
                                    SimpleMode & /**/, SimpleMode & /**/,
                                    TimedMode &generate_mode, SimpleMode & /**/,
                                    StageEngine &engine) {
            controls.defer_ = false;

            controls.gate_ = false; // Start with a step with gate low
            auto constexpr sample_time = 0.0341F;
            engine.process(sample_time);
            t.assert_that(generate_mode.sample_time_, is_equal_to(sample_time));

            controls.gate_ = false; // low + low = low latch
            engine.process(0.F);
            t.assert_that("low", generate_mode.latch_, is_equal_to(low_latch));

            controls.gate_ = true; // low + high = rising latch
            engine.process(0.F);
            t.assert_that("rise", generate_mode.latch_,
                          is_equal_to(rising_latch));

            controls.gate_ = true; // high + high = high latch
            engine.process(0.F);
            t.assert_that("high", generate_mode.latch_,
                          is_equal_to(high_latch));

            controls.gate_ = false; // high + low = falling latch
            engine.process(0.F);
            t.assert_that("fall", generate_mode.latch_,
                          is_equal_to(falling_latch));
          }));

    t.run("if generate completes: raises eoc and enters level mode",
          test(in_generate_mode,
               [](Tester &t, Controls &controls, SimpleMode & /**/,
                  SimpleMode & /**/, TimedMode &generate_mode,
                  SimpleMode &level_mode, StageEngine &engine) {
                 controls.defer_ = false;
                 generate_mode.event_ = Event::Completed;

                 engine.process(0.F);

                 t.assert_that("raised eoc", controls.eoc_, is_true);
                 t.assert_that("execute generate", generate_mode.executed_,
                               is_true);
                 t.assert_that("exit generate", generate_mode.exited_, is_true);
                 t.assert_that("enter level", level_mode.entered_, is_true);
                 t.assert_that("execute level", level_mode.executed_, is_false);
               }));
  }
};
__attribute__((unused)) static auto _ = StageEngineGenerateModeSuite{};
} // namespace stage
} // namespace test
