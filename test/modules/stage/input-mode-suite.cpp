#include "./fixtures/simple-mode-fixture.h"
#include "helpers/assertions.h"
#include "modules/stage/input-mode.h"
#include <dheunit/test.h>

namespace test {
namespace stage {

using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;
using test::is_false;

using InputMode = dhe::stage::InputMode<Controls>;

class InputModeSuite : public Suite {
public:
  InputModeSuite() : Suite{"dhe::stage::InputMode"} {}
  void run(Tester &t) override {
    t.run("enter() deactivates stage",
          test<InputMode>([](Tester &t, Controls &controls, InputMode &mode) {
            controls.active_ = true;
            mode.enter();
            assert_that(t, controls.active_, is_false);
          }));

    t.run("execute() copies input to output",
          test<InputMode>([](Tester &t, Controls &controls, InputMode &mode) {
            auto constexpr input = 0.984F;
            controls.input_ = input;
            mode.execute();
            assert_that(t, controls.output_, is_equal_to(input));
          }));

    t.run("exit() deactivates stage",
          test<InputMode>([](Tester &t, Controls &controls, InputMode &mode) {
            controls.active_ = true;
            mode.exit();
            assert_that(t, controls.active_, is_false);
          }));
  }
};

static auto _ = InputModeSuite{};
} // namespace stage
} // namespace test
