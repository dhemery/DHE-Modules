#include "modules/envelope/mode/input-mode.h"

#include "dheunit/test.h"
#include "helpers/assertions.h"
#include "simple-mode-fixture.h"

namespace test {
namespace envelope {

using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;
using test::is_false;

using InputMode = dhe::envelope::InputMode<Signals>;

class InputModeSuite : public Suite {
public:
  InputModeSuite() : Suite{"dhe::envelope::InputMode"} {}
  void run(Tester &t) override {
    t.run("enter() deactivates stage",
          test<InputMode>([](Tester &t, Signals &signals, InputMode &mode) {
            signals.active_ = true;
            mode.enter();
            assert_that(t, signals.active_, is_false);
          }));

    t.run("execute() copies input to output",
          test<InputMode>([](Tester &t, Signals &signals, InputMode &mode) {
            auto constexpr input = 0.984F;
            signals.input_ = input;
            mode.execute();
            assert_that(t, signals.output_, is_equal_to(input));
          }));

    t.run("exit() deactivates stage",
          test<InputMode>([](Tester &t, Signals &signals, InputMode &mode) {
            signals.active_ = true;
            mode.exit();
            assert_that(t, signals.active_, is_false);
          }));
  }
};

static auto _ = InputModeSuite{};
} // namespace envelope
} // namespace test
