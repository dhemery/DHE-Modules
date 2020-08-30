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
using dhe::unit::TestRegistrar;

template <typename EngineTest>
auto in_generate_mode(EngineTest engine_test) -> TestFunc {
  return [engine_test](Tester &t) {
    Controls controls{};
    SimpleMode defer_mode{};
    SimpleMode input_mode{};
    TimedMode generate_mode{};
    SimpleMode level_mode{};
    StageEngine engine{controls, defer_mode, input_mode, generate_mode,
                       level_mode};
    // Starts in input mode

    // input mode + rising gate -> generate mode
    controls.gate_ = true;
    engine.process(0.F);
    controls.gate_ = false;

    // Reset the fields of the modes we've traversed
    input_mode = SimpleMode{};
    generate_mode = TimedMode{};

    engine_test(t, controls, defer_mode, input_mode, generate_mode, level_mode,
                engine);
  };
}

class StageEngineGenerateModeSuite : Suite {
public:
  StageEngineGenerateModeSuite()
      : Suite{"dhe::stage::StageEngine in generate mode"} {}
  void register_tests(TestRegistrar add) override {
    add("if defer rises: begins deferring",
        test::stage::in_generate_mode([](Tester &t,
                                         test::stage::Controls &controls,
                                         test::stage::SimpleMode &defer_mode,
                                         test::stage::SimpleMode & /**/,
                                         test::stage::TimedMode &generate_mode,
                                         test::stage::SimpleMode & /**/,
                                         test::stage::StageEngine &engine) {
          controls.defer_ = true;

          engine.process(0.F);

          t.assert_that("exit generate", generate_mode.exited_, is_true);
          t.assert_that("execute generate", generate_mode.executed_, is_false);
          t.assert_that("enter defer", defer_mode.entered_, is_true);
          t.assert_that("execute defer", defer_mode.executed_, is_true);
        }));

    add("with defer low: executes regardless of gate",
        test::stage::in_generate_mode(
            [](Tester &t, test::stage::Controls &controls,
               test::stage::SimpleMode & /**/, test::stage::SimpleMode & /**/,
               test::stage::TimedMode &generate_mode,
               test::stage::SimpleMode & /**/,
               test::stage::StageEngine &engine) {
              controls.defer_ = false;

              controls.gate_ = true;
              engine.process(0.F);

              t.assert_that("execute input", generate_mode.executed_, is_true);
              t.assert_that("exit input", generate_mode.exited_, is_false);
              generate_mode.executed_ = false;

              controls.gate_ = true;
              engine.process(0.F);

              t.assert_that("execute input", generate_mode.executed_, is_true);
              t.assert_that("exit input", generate_mode.exited_, is_false);
            }));

    add("passes gate state and sample time to generate.execute()",
        test::stage::in_generate_mode([](Tester &t,
                                         test::stage::Controls &controls,
                                         test::stage::SimpleMode & /**/,
                                         test::stage::SimpleMode & /**/,
                                         test::stage::TimedMode &generate_mode,
                                         test::stage::SimpleMode & /**/,
                                         test::stage::StageEngine &engine) {
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
          t.assert_that("high", generate_mode.latch_, is_equal_to(high_latch));

          controls.gate_ = false; // high + low = falling latch
          engine.process(0.F);
          t.assert_that("fall", generate_mode.latch_,
                        is_equal_to(falling_latch));
        }));

    add("if generate completes: raises eoc and enters level mode",
        test::stage::in_generate_mode(
            [](Tester &t, test::stage::Controls &controls,
               test::stage::SimpleMode & /**/, test::stage::SimpleMode & /**/,
               test::stage::TimedMode &generate_mode,
               test::stage::SimpleMode &level_mode,
               test::stage::StageEngine &engine) {
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
