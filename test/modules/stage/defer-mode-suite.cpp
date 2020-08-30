#include "./fixtures/mode-fixture.h"
#include "modules/stage/defer-mode.h"
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

using DeferMode = dhe::stage::DeferMode<Controls>;

class DeferModeSuite : public Suite {
public:
  DeferModeSuite() : Suite{"dhe::stage::DeferMode"} {}
  void register_tests(TestRegistrar add) override {
    add("enter() activates stage",
        test<DeferMode>([](Tester &t, Controls &controls, DeferMode &mode) {
          controls.active_ = false;
          mode.enter();
          t.assert_that(controls.active_, is_true);
        }));

    add("execute() copies input to output",
        test<DeferMode>([](Tester &t, Controls &controls, DeferMode &mode) {
          auto constexpr input = 0.984F;
          controls.input_ = input;
          mode.execute();
          t.assert_that(controls.output_, is_equal_to(input));
        }));

    add("exit() deactivates stage",
        test<DeferMode>([](Tester &t, Controls &controls, DeferMode &mode) {
          controls.active_ = true;
          mode.exit();
          t.assert_that(controls.active_, is_false);
        }));
  }
};

__attribute__((unused)) static auto _ = DeferModeSuite{};

} // namespace stage
}; // namespace test
