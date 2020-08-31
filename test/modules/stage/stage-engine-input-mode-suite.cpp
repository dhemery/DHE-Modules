#include "./fixtures/stage-engine-fixture.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace stage {
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestRegistrar;

static inline void in_input_mode(Controls & /**/, SimpleMode & /**/,
                                 SimpleMode &input_mode, TimedMode & /**/,
                                 SimpleMode & /**/, StageEngine &engine) {
  // Starts in input mode, but let's run one sample anyway
  engine.process(0.F);
  input_mode = SimpleMode{};
}

class StageEngineInputModeSuite : Suite {
public:
  StageEngineInputModeSuite()
      : Suite{"dhe::stage::StageEngine in input mode"} {}
  void register_tests(TestRegistrar add) override {
    add("if defer rises: begins deferring",
        test(in_input_mode,
             [](Tester &t, Controls &controls, SimpleMode &defer_mode,
                SimpleMode &input_mode, TimedMode & /**/, SimpleMode & /**/,
                StageEngine &engine) {
               controls.defer_ = true;

               engine.process(0.F);

               t.assert_that("exit input", input_mode.exited_, is_true);
               t.assert_that("execute input", input_mode.executed_, is_false);
               t.assert_that("enter defer", defer_mode.entered_, is_true);
               t.assert_that("execute defer", defer_mode.executed_, is_true);
             }));

    add("with defer low: begins generating if gate rises",
        test(in_input_mode, [](Tester &t, Controls &controls, SimpleMode & /**/,
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
        test(in_input_mode, [](Tester &t, Controls &controls, SimpleMode & /**/,
                               SimpleMode &input_mode, TimedMode & /**/,
                               SimpleMode & /**/, StageEngine &engine) {
          controls.defer_ = false;
          controls.gate_ = false;
          engine.process(0.F);
          t.assert_that(input_mode.executed_, is_true);
        }));
  }
};
__attribute__((unused)) static auto _ = StageEngineInputModeSuite{};

} // namespace stage
} // namespace test
