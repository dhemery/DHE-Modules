#include "./fixtures/sequence-controller-fixture.h"
#include "dheunit/assertions.h"

namespace test {
namespace scannable {
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

class SequenceControllerSuite : Suite {
public:
  SequenceControllerSuite() : Suite{"dhe::scannable::SequenceController"} {}

  void register_tests(dhe::unit::TestRegistrar add) override {
    add("blah", test([](Tester &t, Module &module, Generator &generator,
                        SequenceController &sequence_controller) {}));
  }
};
__attribute__((unused)) static auto _ = SequenceControllerSuite{};
} // namespace scannable
} // namespace test
