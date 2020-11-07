#include "./fixtures/engine-fixture.h"
#include "dheunit/assertions.h"

namespace test {
namespace scannibal {
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

class EngineStartSuite : Suite {
public:
  EngineStartSuite() : Suite{"dhe::scannibal::Engine::start()"} {}

  void register_tests(dhe::unit::TestRegistrar add) override {
    add("shows step 0 phase 0.0",
        test([](Tester &t, Module &module, Anchor &start_anchor,
                Anchor &end_anchor, Engine &engine) {
          engine.start();
          t.assert_that(module.step_, is_equal_to(0));
          t.assert_that(module.step_phase_, is_equal_to(0.F));
        }));
  }
};

__attribute__((unused)) static auto start_suite = EngineStartSuite{};
} // namespace scannibal
} // namespace test
