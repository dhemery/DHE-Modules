#include "./fixtures/stage-engine-fixture.h"
#include "modules/stage/stage-engine.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace stage {

using dhe::unit::is_equal_to;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestRegistrar;

class StageEngineSuite : public Suite {
public:
  StageEngineSuite() : Suite{"dhe::stage::StageEngine"} {}
  void register_tests(TestRegistrar add) override {
    add("starts in input mode",
        test([](Tester &t, Controls & /**/, SimpleMode /**/,
                SimpleMode &input_mode, TimedMode & /**/, SimpleMode & /**/,
                StageEngine &engine) {
          engine.process(0.F);
          t.assert_that(input_mode.executed_, is_true);
        }));
  }
};

__attribute__((unused)) static auto _ = StageEngineSuite{};

} // namespace stage
}; // namespace test
