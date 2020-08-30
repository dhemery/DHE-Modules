#include "./fixtures/stage-engine-fixture.h"

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

__attribute__((unused)) static auto engine_suite = StageEngineSuite{};
__attribute__((unused)) static auto input_mode_suite =
    StageEngineInputModeSuite{};
__attribute__((unused)) static auto defer_mode_suite =
    StageEngineDeferModeSuite{};

} // namespace stage
}; // namespace test
