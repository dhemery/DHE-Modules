#include "modules/envelope/mode/idle-mode.h"

#include "dheunit/test.h"
#include "helpers/assertions.h"
#include "simple-mode-fixture.h"

namespace test {
namespace envelope {

using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;
using test::is_false;

using IdleMode = dhe::envelope::IdleMode<Signals>;

class IdleModeSuite : public Suite {
public:
  IdleModeSuite() : Suite{"dhe::envelope::IdleMode"} {}
  void run(Tester &t) override {
    t.run("enter() deactivates stage",
          test<IdleMode>([](Tester &t, Signals &signals, IdleMode &mode) {
            signals.active_ = true;
            mode.enter();
            assert_that(t, signals.active_, is_false);
          }));

    t.run("execute() outputs_ nothing",
          test<IdleMode>([](Tester &t, Signals &signals, IdleMode &mode) {
            auto constexpr original_output = 931.F;
            signals.output_ = original_output;
            mode.execute();
            assert_that(t, signals.output_, is_equal_to(original_output));
          }));

    t.run("exit() deactivates stage",
          test<IdleMode>([](Tester &t, Signals &signals, IdleMode &mode) {
            signals.active_ = true;
            mode.exit();
            assert_that(t, signals.active_, is_false);
          }));
  }
};

static auto _ = IdleModeSuite{};
} // namespace envelope
} // namespace test
