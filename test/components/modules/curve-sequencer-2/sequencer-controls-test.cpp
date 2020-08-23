#include "fake/rack-controls.h"
#include "modules/curve-sequencer-2/controls.h"

#include "dheunit/test.h"

namespace test {
namespace curve_sequencer_2 {
namespace sequence_controls {
using dhe::unit::Suite;
using dhe::unit::Tester;

static auto constexpr step_count{8};

using Controls = dhe::curve_sequencer_2::Controls<fake::Port, fake::Param,
                                                  fake::Light, step_count>;

class ControlsGatedTests : Suite {
public:
  ControlsGatedTests() : Suite{"dhe::curve_sequencer_2::Controls.is_gated()"} {}

  void register_tests(dhe::unit::TestRegistrar add_test) override {
    auto inputs = std::vector<fake::Port>{Controls::InputCount};
    auto outputs = std::vector<fake::Port>{Controls::OutputCount};
    auto params = std::vector<fake::Param>{Controls::ParameterCount};
    auto lights = std::vector<fake::Light>{Controls::LightCount};

    add_test("true if gate button is pressed", [=](Tester &t) mutable {
      Controls controls{inputs, outputs, params, lights};
      params[Controls::GateButton].setValue(1.F);

      if (!controls.is_gated()) {
        t.errorf("got is_gated false, want true");
      }
    });

    add_test("true if gate input is high", [=](Tester &t) mutable {
      Controls controls{inputs, outputs, params, lights};
      inputs[Controls::GateInput].setVoltage(10.F);

      if (!controls.is_gated()) {
        t.errorf("got is_gated false, want true");
      }
    });

    add_test("false if gate input is low and button is not pressed",
             [=](Tester &t) mutable {
               Controls controls{inputs, outputs, params, lights};

               params[Controls::GateButton].setValue(0.F);
               inputs[Controls::GateInput].setVoltage(0.F);

               if (controls.is_gated()) {
                 t.errorf("got is_gated true, want false");
               }
             });
  }
};

__attribute__((unused)) static ControlsGatedTests controls_gated_tests{};

class ControlsLoopingTests : Suite {
public:
  ControlsLoopingTests()
      : Suite{"dhe::curve_sequencer_2::Controls.is_looping()"} {}

  void register_tests(dhe::unit::TestRegistrar add_test) override {
    auto inputs = std::vector<fake::Port>{Controls::InputCount};
    auto outputs = std::vector<fake::Port>{Controls::OutputCount};
    auto params = std::vector<fake::Param>{Controls::ParameterCount};
    auto lights = std::vector<fake::Light>{Controls::LightCount};

    add_test("true if loop button is pressed", [=](Tester &t) mutable {
      Controls controls{inputs, outputs, params, lights};
      params[Controls::LoopButton].setValue(1.F);

      if (!controls.is_looping()) {
        t.errorf("got is_looping false, want true");
      }
    });

    add_test("true if loop input is high", [=](Tester &t) mutable {
      Controls controls{inputs, outputs, params, lights};
      inputs[Controls::LoopInput].setVoltage(10.F);

      if (!controls.is_looping()) {
        t.errorf("got is_looping false, want true");
      }
    });

    add_test("false if loop input is low and button is not pressed",
             [=](Tester &t) mutable {
               Controls controls{inputs, outputs, params, lights};

               params[Controls::LoopButton].setValue(0.F);
               inputs[Controls::LoopInput].setVoltage(0.F);

               if (controls.is_looping()) {
                 t.errorf("got is_looping true, want false");
               }
             });
  }
};

__attribute__((unused)) static ControlsLoopingTests controls_looping_tests{};

} // namespace sequence_controls
} // namespace curve_sequencer_2
} // namespace test
