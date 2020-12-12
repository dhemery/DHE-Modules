#include "./fixtures/simple-mode-fixture.h"
#include "modules/stage/input-mode.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace stage {

using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::Suite;
using dhe::unit::Tester;

using InputMode = dhe::stage::InputMode<Controls>;

class InputModeSuite : public Suite {
public:
  InputModeSuite() : Suite{"dhe::stage::InputMode"} {}
  void run(Tester &t) {
    t.run("enter() deactivates stage",
          test<InputMode>([](Tester &t, Controls &controls, InputMode &mode) {
            controls.active_ = true;
            mode.enter();
            t.assert_that(controls.active_, is_false);
          }));

    t.run("execute() copies input to output",
          test<InputMode>([](Tester &t, Controls &controls, InputMode &mode) {
            auto constexpr input = 0.984F;
            controls.input_ = input;
            mode.execute();
            t.assert_that(controls.output_, is_equal_to(input));
          }));

    t.run("exit() deactivates stage",
          test<InputMode>([](Tester &t, Controls &controls, InputMode &mode) {
            controls.active_ = true;
            mode.exit();
            t.assert_that(controls.active_, is_false);
          }));
  }
};

__attribute__((unused)) static auto _ = InputModeSuite{};

} // namespace stage
}; // namespace test
