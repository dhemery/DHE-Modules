#include "./fixtures/stage-engine-fixture.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace stage {
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;

static inline void in_level_mode(Controls &controls, SimpleMode & /**/,
                                 SimpleMode &input_mode,
                                 TimedMode &generate_mode,
                                 SimpleMode &level_mode, StageEngine &engine) {
  // Starts in input mode

  // input mode + gate rise -> generate mode
  controls.gate_ = true;
  engine.process(0.F);
  controls.gate_ = false;

  // generate mode + generate completes -> level mode
  generate_mode.event_ = Event::Completed;
  engine.process(0.F);

  // Reset the fields of the modes we've traversed
  input_mode = SimpleMode{};
  generate_mode = TimedMode{};
  level_mode = SimpleMode{};
}

class StageEngineLevelModeSuite : Suite {
public:
  StageEngineLevelModeSuite()
      : Suite{"dhe::stage::StageEngine in level mode"} {}
  void run(Tester &t) override {
    t.run("if defer rises: begins deferring",
          test(in_level_mode,
               [](Tester &t, Controls &controls, SimpleMode &defer_mode,
                  SimpleMode & /**/, TimedMode & /**/, SimpleMode &level_mode,
                  StageEngine &engine) {
                 controls.defer_ = true;

                 engine.process(0.F);

                 t.assert_that("exit level", level_mode.exited_, is_true);
                 t.assert_that("execute level", level_mode.executed_, is_false);
                 t.assert_that("enter defer", defer_mode.entered_, is_true);
                 t.assert_that("execute defer", defer_mode.executed_, is_true);
               }));

    t.run(
        "with defer low: begins generating if gate rises",
        test(in_level_mode, [](Tester &t, Controls &controls, SimpleMode & /**/,
                               SimpleMode & /**/, TimedMode &generate_mode,
                               SimpleMode &level_mode, StageEngine &engine) {
          controls.defer_ = false;
          controls.gate_ = true;

          engine.process(0.F);

          t.assert_that("exit level", level_mode.exited_, is_true);
          t.assert_that("execute level", level_mode.executed_, is_false);
          t.assert_that("enter generate", generate_mode.entered_, is_true);
          t.assert_that("execute generate", generate_mode.executed_, is_true);
        }));

    t.run(
        "with defer low: executes if gate does not rise",
        test(in_level_mode, [](Tester &t, Controls &controls, SimpleMode & /**/,
                               SimpleMode & /**/, TimedMode & /**/,
                               SimpleMode &level_mode, StageEngine &engine) {
          controls.defer_ = false;
          controls.gate_ = false;
          engine.process(0.F);
          t.assert_that(level_mode.executed_, is_true);
        }));
  }
};

__attribute__((unused)) static auto _ = StageEngineLevelModeSuite{};
} // namespace stage
} // namespace test
