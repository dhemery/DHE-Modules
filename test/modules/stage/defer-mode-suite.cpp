#include "./fixtures/simple-mode-fixture.h"
#include "helpers/assertions.h"
#include "modules/stage/defer-mode.h"
#include <dheunit/test.h>

namespace test {
namespace stage {

using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;
using test::is_false;
using test::is_true;

using DeferMode = dhe::stage::DeferMode<Controls>;

class DeferModeSuite : public Suite {
public:
  DeferModeSuite() : Suite{"dhe::stage::DeferMode"} {}
  void run(Tester &t) override {
    t.run("enter() activates stage",
          test<DeferMode>([](Tester &t, Controls &controls, DeferMode &mode) {
            controls.active_ = false;
            mode.enter();
            assert_that(t, controls.active_, is_true);
          }));

    t.run("execute() copies input to output",
          test<DeferMode>([](Tester &t, Controls &controls, DeferMode &mode) {
            auto constexpr input = 0.984F;
            controls.input_ = input;
            mode.execute();
            assert_that(t, controls.output_, is_equal_to(input));
          }));

    t.run("exit() deactivates stage",
          test<DeferMode>([](Tester &t, Controls &controls, DeferMode &mode) {
            controls.active_ = true;
            mode.exit();
            assert_that(t, controls.active_, is_false);
          }));
  }
};

__attribute__((unused)) static auto _ = DeferModeSuite{};

} // namespace stage
}; // namespace test
