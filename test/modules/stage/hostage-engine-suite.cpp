#include "./fixtures/hostage-engine-fixture.h"

#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace stage {

using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestRegistrar;

// Nothing to do, because the state is already freshly constructed
static inline void
freshly_constructed(Controls & /*controls*/, SimpleMode & /*input_mode*/,
                    SimpleMode & /*defer_mode*/, TimedMode & /*hold_mode*/,
                    LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                    HostageEngine & /*engine*/) {}

class HostageEngineSuite : public Suite {
public:
  HostageEngineSuite() : Suite{"dhe::stage::HostageEngine"} {}
  void register_tests(TestRegistrar add) override {
    add("starts in input mode",
        test(freshly_constructed,
             [](Tester &t, Controls & /*controls*/, SimpleMode &input_mode,
                SimpleMode & /*defer_mode*/, TimedMode & /*hold_mode*/,
                LatchedMode & /*sustain_mode*/, SimpleMode & /*idle_mode*/,
                HostageEngine &engine) {
               engine.process(0.F);
               t.assert_that(input_mode.executed_, is_true);
             }));
  }
};

__attribute__((unused)) static auto _ = HostageEngineSuite{};

} // namespace stage
}; // namespace test
