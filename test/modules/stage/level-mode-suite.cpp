#include "./fixtures/simple-mode-fixture.h"
#include "modules/stage/level-mode.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace stage {

using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::Suite;
using dhe::unit::Tester;

using LevelMode = dhe::stage::LevelMode<Controls>;

class LevelModeSuite : public Suite {
public:
  LevelModeSuite() : Suite{"dhe::stage::LevelMode"} {}
  void run(Tester &t) {
    t.run("enter() deactivates stage",
          test<LevelMode>([](Tester &t, Controls &controls, LevelMode &mode) {
            controls.active_ = true;
            mode.enter();
            t.assert_that(controls.active_, is_false);
          }));

    t.run("execute() copies level to output",
          test<LevelMode>([](Tester &t, Controls &controls, LevelMode &mode) {
            auto constexpr level = 8.348F;
            controls.level_ = level;
            mode.execute();
            t.assert_that(controls.output_, is_equal_to(level));
          }));

    t.run("exit() deactivates stage",
          test<LevelMode>([](Tester &t, Controls &controls, LevelMode &mode) {
            controls.active_ = true;
            mode.exit();
            t.assert_that(controls.active_, is_false);
          }));
  }
};

__attribute__((unused)) static auto _ = LevelModeSuite{};

} // namespace stage
}; // namespace test
