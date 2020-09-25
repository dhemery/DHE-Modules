#include "./fixtures/controls-fixture.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {

using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestRegistrar;

class ControlsSuite : public Suite {
public:
  ControlsSuite() : Suite("dhe::curve_sequencer::Controls") {}
  void register_tests(TestRegistrar add) override {
    add("is_gated() is true if gate button is pressed",
        test([](Tester &t, Module &module, Controls &controls) {
          module.params_[Param::GateButton].setValue(1.F);

          t.assert_that(controls.is_gated(), is_true);
        }));

    add("is_gated() is true if gate input is high",
        test([](Tester &t, Module &module, Controls &controls) {
          module.inputs_[Input::GateInput].setVoltage(10.F);

          t.assert_that(controls.is_gated(), is_true);
        }));

    add("is_gated() is false if gate input is low and gate button is not "
        "pressed",
        test([](Tester &t, Module &module, Controls &controls) {
          module.params_[Param ::GateButton].setValue(0.F);
          module.inputs_[Input::GateInput].setVoltage(0.F);

          t.assert_that(controls.is_gated(), is_false);
        }));
  }
};

static auto _ = ControlsSuite{};

} // namespace curve_sequencer
} // namespace test
