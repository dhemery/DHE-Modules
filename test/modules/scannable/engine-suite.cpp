#include "./fixtures/engine-fixture.h"
#include "dheunit/assertions.h"

namespace test {
namespace scannable {
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

class EngineSuite : Suite {
public:
  EngineSuite() : Suite{"dhe::scannable::Engine"} {}

  void register_tests(dhe::unit::TestRegistrar add) override {
    add("blah", test([](Tester &t, Module &module, Anchor &start_anchor,
                        Anchor &end_anchor, Engine &engine) {}));
  }
};

__attribute__((unused)) static auto _ = EngineSuite{};
} // namespace scannable
} // namespace test
