#include "modules/envelope/mode/level.h"

#include "dheunit/test.h"
#include "helpers/assertions.h"
#include "simple-mode-fixture.h"

namespace test {
namespace envelope {

using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;
using test::is_false;

using LevelMode = dhe::envelope::mode::LevelMode<Signals>;

class LevelModeSuite : public Suite {
public:
  LevelModeSuite() : Suite{"dhe::envelope::mode::Level"} {}
  void run(Tester &t) override {
    t.run("enter() deactivates stage",
          test<LevelMode>([](Tester &t, Signals &signals, LevelMode &mode) {
            signals.active_ = true;
            mode.enter();
            assert_that(t, signals.active_, is_false);
          }));

    t.run("execute() copies level to output",
          test<LevelMode>([](Tester &t, Signals &signals, LevelMode &mode) {
            auto constexpr level = 8.348F;
            signals.level_ = level;
            mode.execute();
            assert_that(t, signals.output_, is_equal_to(level));
          }));

    t.run("exit() deactivates stage",
          test<LevelMode>([](Tester &t, Signals &signals, LevelMode &mode) {
            signals.active_ = true;
            mode.exit();
            assert_that(t, signals.active_, is_false);
          }));
  }
};

static auto _ = LevelModeSuite{};
} // namespace envelope
} // namespace test
