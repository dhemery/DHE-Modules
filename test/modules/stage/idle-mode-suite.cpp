#include "./fixtures/simple-mode-fixture.h"
#include "modules/stage/idle-mode.h"

#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace stage {

using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestRegistrar;

using IdleMode = dhe::stage::IdleMode<Controls>;

class IdleModeSuite : public Suite {
public:
  IdleModeSuite() : Suite{"dhe::stage::IdleMode"} {}
  void register_tests(TestRegistrar add) override {
    add("enter() deactivates stage",
        test<IdleMode>([](Tester &t, Controls &controls, IdleMode &mode) {
          controls.active_ = true;
          mode.enter();
          t.assert_that(controls.active_, is_false);
        }));

    add("execute() outputs nothing",
        test<IdleMode>([](Tester &t, Controls &controls, IdleMode &mode) {
          auto constexpr original_output = 931.F;
          controls.output_ = original_output;
          mode.execute();
          t.assert_that(controls.output_, is_equal_to(original_output));
        }));

    add("exit() deactivates stage",
        test<IdleMode>([](Tester &t, Controls &controls, IdleMode &mode) {
          controls.active_ = true;
          mode.exit();
          t.assert_that(controls.active_, is_false);
        }));
  }
};

__attribute__((unused)) static auto _ = IdleModeSuite{};

} // namespace stage
}; // namespace test
