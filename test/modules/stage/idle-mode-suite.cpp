#include "modules/stage/idle-mode.h"

#include "./fixtures/simple-mode-fixture.h"
#include "helpers/assertions.h"

#include "dheunit/test.h"

namespace test {
namespace stage {

using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;
using test::is_false;

using IdleMode = dhe::stage::IdleMode<Controls>;

class IdleModeSuite : public Suite {
public:
  IdleModeSuite() : Suite{"dhe::stage::IdleMode"} {}
  void run(Tester &t) override {
    t.run("enter() deactivates stage",
          test<IdleMode>([](Tester &t, Controls &controls, IdleMode &mode) {
            controls.active_ = true;
            mode.enter();
            assert_that(t, controls.active_, is_false);
          }));

    t.run("execute() outputs_ nothing",
          test<IdleMode>([](Tester &t, Controls &controls, IdleMode &mode) {
            auto constexpr original_output = 931.F;
            controls.output_ = original_output;
            mode.execute();
            assert_that(t, controls.output_, is_equal_to(original_output));
          }));

    t.run("exit() deactivates stage",
          test<IdleMode>([](Tester &t, Controls &controls, IdleMode &mode) {
            controls.active_ = true;
            mode.exit();
            assert_that(t, controls.active_, is_false);
          }));
  }
};

static auto _ = IdleModeSuite{};
} // namespace stage
} // namespace test
