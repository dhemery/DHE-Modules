#include "dheunit/test.h"
#include "helpers/assertions.h"
#include "stage-engine-fixture.h"

namespace test {
namespace stage {

using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_false;
using test::is_true;

static inline void in_defer_mode(Signals &signals, SimpleMode &defer_mode,
                                 SimpleMode &input_mode, TimedMode & /**/,
                                 SimpleMode & /**/, StageEngine &engine) {
  // Starts in input mode

  // input mode + defer rise -> defer mode
  signals.defer_ = true;
  engine.process(0.F);
  // leave defer high

  // Reset the fields of the modes we've traversed
  input_mode = SimpleMode{};
  defer_mode = SimpleMode{};
}

class StageEngineDeferModeSuite : Suite {
public:
  StageEngineDeferModeSuite()
      : Suite{"dhe::envelope::stage::Engine in defer mode"} {}
  void run(Tester &t) override {
    t.run("with defer high: executes regardless of gate",
          test(in_defer_mode, [](Tester &t, Signals &signals,
                                 SimpleMode &defer_mode, SimpleMode & /**/,
                                 TimedMode & /**/, SimpleMode & /**/,
                                 StageEngine &engine) {
            signals.defer_ = true;

            signals.gate_ = true;
            engine.process(0.F);
            assert_that(t, "with gate high", defer_mode.executed_, is_true);

            defer_mode.executed_ = false;
            signals.gate_ = false;
            engine.process(0.F);
            assert_that(t, "with gate low", defer_mode.executed_, is_true);
          }));

    t.run("if defer falls with gate high: begins generating",
          test(in_defer_mode, [](Tester &t, Signals &signals,
                                 SimpleMode &defer_mode, SimpleMode & /**/,
                                 TimedMode &generate_mode, SimpleMode & /**/,
                                 StageEngine &engine) {
            signals.defer_ = false;
            signals.gate_ = true;

            engine.process(0.F);

            assert_that(t, "exit defer", defer_mode.exited_, is_true);
            assert_that(t, "execute defer", defer_mode.executed_, is_false);
            assert_that(t, "enter generate", generate_mode.entered_, is_true);
            assert_that(t, "execute generate", generate_mode.executed_,
                        is_true);
          }));

    t.run("if defer falls with gate low: begins tracking input without raising "
          "eoc",
          test(in_defer_mode, [](Tester &t, Signals &signals,
                                 SimpleMode &defer_mode, SimpleMode &input_mode,
                                 TimedMode & /**/, SimpleMode & /**/,
                                 StageEngine &engine) {
            signals.defer_ = false;
            signals.gate_ = false;

            engine.process(0.F);

            assert_that(t, "exit defer", defer_mode.exited_, is_true);
            assert_that(t, "execute defer", defer_mode.executed_, is_false);
            assert_that(t, "enter input", input_mode.entered_, is_true);
            assert_that(t, "execute input", input_mode.executed_, is_true);
            assert_that(t, "raised eoc", signals.eoc_, is_false);
          }));
  }
};

static auto _ = StageEngineDeferModeSuite{};
} // namespace stage
} // namespace test
