#include "modules/envelope/mode/defer.h"

#include "dheunit/test.h"
#include "helpers/assertions.h"
#include "simple-mode-fixture.h"

namespace test {
namespace envelope {

using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;
using test::is_false;
using test::is_true;

using DeferMode = dhe::envelope::mode::DeferMode<Signals>;

class DeferModeSuite : public Suite {
public:
  DeferModeSuite() : Suite{"dhe::envelope::mode::Defer"} {}
  void run(Tester &t) override {
    t.run("enter() activates stage",
          test<DeferMode>([](Tester &t, Signals &signals, DeferMode &mode) {
            signals.active_ = false;
            mode.enter();
            assert_that(t, signals.active_, is_true);
          }));

    t.run("execute() copies input to output",
          test<DeferMode>([](Tester &t, Signals &signals, DeferMode &mode) {
            auto constexpr input = 0.984F;
            signals.input_ = input;
            mode.execute();
            assert_that(t, signals.output_, is_equal_to(input));
          }));

    t.run("exit() deactivates stage",
          test<DeferMode>([](Tester &t, Signals &signals, DeferMode &mode) {
            signals.active_ = true;
            mode.exit();
            assert_that(t, signals.active_, is_false);
          }));
  }
};

static auto _ = DeferModeSuite{};

} // namespace envelope
} // namespace test
