#include "dheunit/test.h"
#include "helpers/assertions.h"
#include "hostage-engine-fixture.h"

namespace test {
namespace hostage {

using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_true;

// Nothing to do, because the state is already freshly constructed
static inline void
freshly_constructed(Signals & /*signals*/, SimpleMode & /*input_mode*/,
                    SimpleMode & /*defer_mode*/, TimedMode & /*hold_mode*/,
                    LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                    HostageEngine & /*engine*/) {}

class HostageEngineSuite : public Suite {
public:
  HostageEngineSuite() : Suite{"dhe::envelope::hostage::Engine"} {}
  void run(Tester &t) override {
    t.run("starts in input mode",
          test(freshly_constructed,
               [](Tester &t, Signals & /*signals*/, SimpleMode &input_mode,
                  SimpleMode & /*defer_mode*/, TimedMode & /*hold_mode*/,
                  LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                  HostageEngine &engine) {
                 engine.process(0.F);
                 assert_that(t, input_mode.executed_, is_true);
               }));
  }
};

static auto _ = HostageEngineSuite{};
} // namespace hostage
} // namespace test
