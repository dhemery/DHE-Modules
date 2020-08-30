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

class StageEngineSuite : public Suite {
public:
  StageEngineSuite() : Suite{"dhe::stage::StageEngine"} {}
  void register_tests(TestRegistrar add) override {
    add("starts in input mode",
        test([](Tester &t, Controls & /**/, SimpleMode /**/,
                SimpleMode &input_mode, TimedMode & /**/, SimpleMode & /**/,
                StageEngine &engine) {
          engine.process(0.F);
          t.assert_that(input_mode.executed_, is_true);
        }));
  }
};

class StageEngineInputModeSuite : Suite {
public:
  StageEngineInputModeSuite()
      : Suite{"dhe::stage::StageEngine in input mode"} {}
  void register_tests(TestRegistrar add) override {
    add("if defer rises: begins deferring",
        in_input_mode([](Tester &t, Controls &controls, SimpleMode &defer_mode,
                         SimpleMode &input_mode, TimedMode & /**/,
                         SimpleMode & /**/, StageEngine &engine) {
          controls.defer_ = true;

          engine.process(0.F);

          t.assert_that("exit input", input_mode.exited_, is_true);
          t.assert_that("execute input", input_mode.executed_, is_false);
          t.assert_that("enter defer", defer_mode.entered_, is_true);
          t.assert_that("execute defer", defer_mode.executed_, is_true);
        }));

    add("with defer low: begins generating if gate rises",
        in_input_mode([](Tester &t, Controls &controls, SimpleMode & /**/,
                         SimpleMode &input_mode, TimedMode &generate_mode,
                         SimpleMode & /**/, StageEngine &engine) {
          controls.defer_ = false;
          controls.gate_ = true;

          engine.process(0.F);

          t.assert_that("exit input", input_mode.exited_, is_true);
          t.assert_that("execute input", input_mode.executed_, is_false);
          t.assert_that("enter generate", generate_mode.entered_, is_true);
          t.assert_that("execuge generate", generate_mode.executed_, is_true);
        }));

    add("with defer low: executes if gate does not rise",
        in_input_mode([](Tester &t, Controls &controls, SimpleMode & /**/,
                         SimpleMode &input_mode, TimedMode & /**/,
                         SimpleMode & /**/, StageEngine &engine) {
          controls.defer_ = false;
          controls.gate_ = false;
          engine.process(0.F);
          t.assert_that(input_mode.executed_, is_true);
        }));
  }
};

class StageEngineDeferModeSuite : Suite {
public:
  StageEngineDeferModeSuite()
      : Suite{"dhe::stage::StageEngine in defer mode"} {}
  void register_tests(TestRegistrar add) override {
    add("with defer high: executes regardless of gate",
        in_defer_mode([](Tester &t, Controls &controls, SimpleMode &defer_mode,
                         SimpleMode & /**/, TimedMode & /**/, SimpleMode & /**/,
                         StageEngine &engine) {
          controls.defer_ = true;

          controls.gate_ = true;
          engine.process(0.F);
          t.assert_that("with gate high", defer_mode.executed_, is_true);

          defer_mode.executed_ = false;
          controls.gate_ = false;
          engine.process(0.F);
          t.assert_that("with gate low", defer_mode.executed_, is_true);
        }));

    add("if defer falls with gate high: begins generating",
        in_defer_mode([](Tester &t, Controls &controls, SimpleMode &defer_mode,
                         SimpleMode & /**/, TimedMode &generate_mode,
                         SimpleMode & /**/, StageEngine &engine) {
          controls.defer_ = false;
          controls.gate_ = true;

          engine.process(0.F);

          t.assert_that("exit defer", defer_mode.exited_, is_true);
          t.assert_that("execute defer", defer_mode.executed_, is_false);
          t.assert_that("enter generate", generate_mode.entered_, is_true);
          t.assert_that("execuge generate", generate_mode.executed_, is_true);
        }));

    add("if defer falls with gate low: begins tracking input without raising "
        "eoc",
        in_defer_mode([](Tester &t, Controls &controls, SimpleMode &defer_mode,
                         SimpleMode &input_mode, TimedMode & /**/,
                         SimpleMode & /**/, StageEngine &engine) {
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

class StageEngineGenerateModeSuite : Suite {
public:
  StageEngineGenerateModeSuite()
      : Suite{"dhe::stage::StageEngine in generate mode"} {}
  void register_tests(TestRegistrar add) override {
    add("if defer rises: begins deferring",
        in_generate_mode([](Tester &t, Controls &controls,
                            SimpleMode &defer_mode, SimpleMode & /**/,
                            TimedMode &generate_mode, SimpleMode & /**/,
                            StageEngine &engine) {
          controls.defer_ = true;

          engine.process(0.F);

          t.assert_that("exit generate", generate_mode.exited_, is_true);
          t.assert_that("execute generate", generate_mode.executed_, is_false);
          t.assert_that("enter defer", defer_mode.entered_, is_true);
          t.assert_that("execute defer", defer_mode.executed_, is_true);
        }));

    add("with defer low: executes regardless of gate",
        in_generate_mode([](Tester &t, Controls &controls, SimpleMode & /**/,
                            SimpleMode & /**/, TimedMode &generate_mode,
                            SimpleMode & /**/, StageEngine &engine) {
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
        in_generate_mode([](Tester &t, Controls &controls, SimpleMode & /**/,
                            SimpleMode & /**/, TimedMode &generate_mode,
                            SimpleMode & /**/, StageEngine &engine) {
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
        in_generate_mode([](Tester &t, Controls &controls, SimpleMode & /**/,
                            SimpleMode & /**/, TimedMode &generate_mode,
                            SimpleMode &level_mode, StageEngine &engine) {
          controls.defer_ = false;
          generate_mode.event_ = Event::Completed;

          engine.process(0.F);

          t.assert_that("raised eoc", controls.eoc_, is_true);
          t.assert_that("execute generate", generate_mode.executed_, is_true);
          t.assert_that("exit generate", generate_mode.exited_, is_true);
          t.assert_that("enter level", level_mode.entered_, is_true);
          t.assert_that("execute level", level_mode.executed_, is_false);
        }));
  }
};

class StageEngineLevelModeSuite : Suite {
public:
  StageEngineLevelModeSuite()
      : Suite{"dhe::stage::StageEngine in level mode"} {}
  void register_tests(TestRegistrar add) override {
    add("if defer rises: begins deferring",
        in_level_mode([](Tester &t, Controls &controls, SimpleMode &defer_mode,
                         SimpleMode & /**/, TimedMode & /**/,
                         SimpleMode &level_mode, StageEngine &engine) {
          controls.defer_ = true;

          engine.process(0.F);

          t.assert_that("exit level", level_mode.exited_, is_true);
          t.assert_that("execute level", level_mode.executed_, is_false);
          t.assert_that("enter defer", defer_mode.entered_, is_true);
          t.assert_that("execute defer", defer_mode.executed_, is_true);
        }));

    add("with defer low: begins generating if gate rises",
        in_level_mode([](Tester &t, Controls &controls, SimpleMode & /**/,
                         SimpleMode & /**/, TimedMode &generate_mode,
                         SimpleMode &level_mode, StageEngine &engine) {
          controls.defer_ = false;
          controls.gate_ = true;

          engine.process(0.F);

          t.assert_that("exit level", level_mode.exited_, is_true);
          t.assert_that("execute level", level_mode.executed_, is_false);
          t.assert_that("enter generate", generate_mode.entered_, is_true);
          t.assert_that("execuge generate", generate_mode.executed_, is_true);
        }));

    add("with defer low: executes if gate does not rise",
        in_level_mode([](Tester &t, Controls &controls, SimpleMode & /**/,
                         SimpleMode & /**/, TimedMode & /**/,
                         SimpleMode &level_mode, StageEngine &engine) {
          controls.defer_ = false;
          controls.gate_ = false;
          engine.process(0.F);
          t.assert_that(level_mode.executed_, is_true);
        }));
  }
};

__attribute__((unused)) static auto engine_suite = StageEngineSuite{};
__attribute__((unused)) static auto input_mode_suite =
    StageEngineInputModeSuite{};
__attribute__((unused)) static auto defer_mode_suite =
    StageEngineDeferModeSuite{};
__attribute__((unused)) static auto generate_mode_suite =
    StageEngineGenerateModeSuite{};
__attribute__((unused)) static auto level_mode_suite =
    StageEngineLevelModeSuite{};

} // namespace stage
}; // namespace test
