#include "./fixtures/sequence-controller-fixture.h"
#include "dheunit/assertions.h"

namespace test {
namespace curve_sequencer {
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

class SequenceControllerSuite : Suite {
public:
  SequenceControllerSuite()
      : Suite{"dhe::curve_sequencer::SequenceController"} {}

  void register_tests(dhe::unit::TestRegistrar add) override {
    add("foo",
        test([](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {}));
  }
};
__attribute__((unused)) static auto _ = SequenceControllerSuite{};
} // namespace curve_sequencer
} // namespace test