#include "./fixtures/sequence-controller-fixture.h"
#include "dheunit/assertions.h"

namespace test {
namespace scannibal {
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

class SequenceControllerStartSuite : Suite {
public:
  SequenceControllerStartSuite()
      : Suite{"dhe::scannibal::Controller::start()"} {}

  void register_tests(dhe::unit::TestRegistrar add) override {
    add("shows step 0 phase 0.0",
        test([](Tester &t, Module &module, Generator &generator,
                Controller &controller) {}));
  }
};

__attribute__((unused)) static auto start_suite =
    SequenceControllerStartSuite{};
} // namespace scannibal
} // namespace test
