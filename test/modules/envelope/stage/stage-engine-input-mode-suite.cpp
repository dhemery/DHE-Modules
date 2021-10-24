#include "dheunit/test.h"
#include "helpers/assertions.h"
#include "stage-engine-fixture.h"

namespace test {
namespace envelope {
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_false;
using test::is_true;

static inline void in_input_mode(Signals & /**/, SimpleMode & /**/,
                                 SimpleMode &input_mode, TimedMode & /**/,
                                 SimpleMode & /**/, StageEngine &engine) {
  // Starts in input mode, but let's run one sample anyway
  engine.process(0.F);
  input_mode = SimpleMode{};
}

class StageEngineInputModeSuite : Suite {
public:
  StageEngineInputModeSuite()
      : Suite{"dhe::envelope::stage::Engine in input mode"} {}
  void run(Tester &t) override {
    t.run("if defer rises: begins deferring",
          test(in_input_mode, [](Tester &t, Signals &signals,
                                 SimpleMode &defer_mode, SimpleMode &input_mode,
                                 TimedMode & /**/, SimpleMode & /**/,
                                 StageEngine &engine) {
            signals.defer_ = true;

            engine.process(0.F);

            assert_that(t, "exit input", input_mode.exited_, is_true);
            assert_that(t, "execute input", input_mode.executed_, is_false);
            assert_that(t, "enter defer", defer_mode.entered_, is_true);
            assert_that(t, "execute defer", defer_mode.executed_, is_true);
          }));

    t.run(
        "with defer low: begins generating if gate rises",
        test(in_input_mode, [](Tester &t, Signals &signals, SimpleMode & /**/,
                               SimpleMode &input_mode, TimedMode &generate_mode,
                               SimpleMode & /**/, StageEngine &engine) {
          signals.defer_ = false;
          signals.gate_ = true;

          engine.process(0.F);

          assert_that(t, "exit input", input_mode.exited_, is_true);
          assert_that(t, "execute input", input_mode.executed_, is_false);
          assert_that(t, "enter generate", generate_mode.entered_, is_true);
          assert_that(t, "execute generate", generate_mode.executed_, is_true);
        }));

    t.run("with defer low: executes if gate does not rise",
          test(in_input_mode, [](Tester &t, Signals &signals, SimpleMode & /**/,
                                 SimpleMode &input_mode, TimedMode & /**/,
                                 SimpleMode & /**/, StageEngine &engine) {
            signals.defer_ = false;
            signals.gate_ = false;
            engine.process(0.F);
            assert_that(t, input_mode.executed_, is_true);
          }));
  }
};

static auto _ = StageEngineInputModeSuite{};
} // namespace envelope
} // namespace test
