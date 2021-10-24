#include "dheunit/test.h"
#include "helpers/assertions.h"
#include "stage-engine-fixture.h"

namespace test {
namespace stage {
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_false;
using test::is_true;

static inline void in_level_mode(Signals &signals, SimpleMode & /**/,
                                 SimpleMode &input_mode,
                                 TimedMode &generate_mode,
                                 SimpleMode &level_mode, StageEngine &engine) {
  // Starts in input mode

  // input mode + gate rise -> generate mode
  signals.gate_ = true;
  engine.process(0.F);
  signals.gate_ = false;

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
      : Suite{"dhe::envelope::stage::Engine in level mode"} {}
  void run(Tester &t) override {
    t.run("if defer rises: begins deferring",
          test(in_level_mode, [](Tester &t, Signals &signals,
                                 SimpleMode &defer_mode, SimpleMode & /**/,
                                 TimedMode & /**/, SimpleMode &level_mode,
                                 StageEngine &engine) {
            signals.defer_ = true;

            engine.process(0.F);

            assert_that(t, "exit level", level_mode.exited_, is_true);
            assert_that(t, "execute level", level_mode.executed_, is_false);
            assert_that(t, "enter defer", defer_mode.entered_, is_true);
            assert_that(t, "execute defer", defer_mode.executed_, is_true);
          }));

    t.run("with defer low: begins generating if gate rises",
          test(in_level_mode, [](Tester &t, Signals &signals, SimpleMode & /**/,
                                 SimpleMode & /**/, TimedMode &generate_mode,
                                 SimpleMode &level_mode, StageEngine &engine) {
            signals.defer_ = false;
            signals.gate_ = true;

            engine.process(0.F);

            assert_that(t, "exit level", level_mode.exited_, is_true);
            assert_that(t, "execute level", level_mode.executed_, is_false);
            assert_that(t, "enter generate", generate_mode.entered_, is_true);
            assert_that(t, "execute generate", generate_mode.executed_,
                        is_true);
          }));

    t.run("with defer low: executes if gate does not rise",
          test(in_level_mode, [](Tester &t, Signals &signals, SimpleMode & /**/,
                                 SimpleMode & /**/, TimedMode & /**/,
                                 SimpleMode &level_mode, StageEngine &engine) {
            signals.defer_ = false;
            signals.gate_ = false;
            engine.process(0.F);
            assert_that(t, level_mode.executed_, is_true);
          }));
  }
};

static auto _ = StageEngineLevelModeSuite{};
} // namespace stage
} // namespace test
