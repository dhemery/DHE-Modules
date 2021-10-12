#include "modules/stage/level-mode.h"

#include "./fixtures/simple-mode-fixture.h"
#include "helpers/assertions.h"

#include "dheunit/test.h"

namespace test {
namespace stage {

using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;
using test::is_false;

using LevelMode = dhe::stage::LevelMode<Controls>;

class LevelModeSuite : public Suite {
public:
  LevelModeSuite() : Suite{"dhe::stage::LevelMode"} {}
  void run(Tester &t) override {
    t.run("enter() deactivates stage",
          test<LevelMode>([](Tester &t, Controls &controls, LevelMode &mode) {
            controls.active_ = true;
            mode.enter();
            assert_that(t, controls.active_, is_false);
          }));

    t.run("execute() copies level to output",
          test<LevelMode>([](Tester &t, Controls &controls, LevelMode &mode) {
            auto constexpr level = 8.348F;
            controls.level_ = level;
            mode.execute();
            assert_that(t, controls.output_, is_equal_to(level));
          }));

    t.run("exit() deactivates stage",
          test<LevelMode>([](Tester &t, Controls &controls, LevelMode &mode) {
            controls.active_ = true;
            mode.exit();
            assert_that(t, controls.active_, is_false);
          }));
  }
};

static auto _ = LevelModeSuite{};
} // namespace stage
} // namespace test
