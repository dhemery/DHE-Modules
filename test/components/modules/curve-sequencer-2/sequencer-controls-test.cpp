#include "fake/rack-controls.h"
#include "modules/curve-sequencer-2/controls.h"

#include "dheunit/test.h"

namespace test {
namespace curve_sequencer_2 {
namespace sequence_controls {
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

static auto constexpr step_count{8};

using Controls = dhe::curve_sequencer_2::Controls<fake::Port, fake::Param,
                                                  fake::Light, step_count>;

auto test_input_button_pair(Controls::InputIds input,
                            Controls::ParameterIds button,
                            std::function<bool(Controls const &)> const &getter)
    -> TestFunc {
  return [input, button, getter](Tester &t) {
    auto inputs = std::vector<fake::Port>{Controls::InputCount};
    auto outputs = std::vector<fake::Port>{Controls::OutputCount};
    auto params = std::vector<fake::Param>{Controls::ParameterCount};
    auto lights = std::vector<fake::Light>{Controls::LightCount};

    Controls controls{inputs, outputs, params, lights};

    params[button].setValue(1.F);
    inputs[input].setVoltage(0.F);
    if (!getter(controls)) {
      t.error("got false with button pressed, want true");
    }

    params[button].setValue(0.F);
    inputs[input].setVoltage(10.F);
    if (!getter(controls)) {
      t.error("got false with input high, want true");
    }

    params[button].setValue(0.F);
    inputs[input].setVoltage(0.F);
    if (getter(controls)) {
      t.error("got true with input low and button released, want false");
    }
  };
}

class ControlsGatedTests : Suite {
public:
  ControlsGatedTests() : Suite{"dhe::curve_sequencer_2::Controls"} {}

  void register_tests(dhe::unit::TestRegistrar add_test) override {
    add_test("is_gated()",
             test_input_button_pair(
                 Controls::GateInput, Controls::GateButton,
                 [](Controls const &c) -> bool { return c.is_gated(); }));
    add_test("is_looping()",
             test_input_button_pair(
                 Controls::LoopInput, Controls::LoopButton,
                 [](Controls const &c) -> bool { return c.is_looping(); }));
    add_test("is_reset()",
             test_input_button_pair(
                 Controls::ResetInput, Controls::ResetButton,
                 [](Controls const &c) -> bool { return c.is_reset(); }));
    add_test("is_running()",
             test_input_button_pair(
                 Controls::RunInput, Controls::RunButton,
                 [](Controls const &c) -> bool { return c.is_running(); }));
  }
};

__attribute__((unused)) static ControlsGatedTests controls_gated_tests{};

} // namespace sequence_controls
} // namespace curve_sequencer_2
} // namespace test
