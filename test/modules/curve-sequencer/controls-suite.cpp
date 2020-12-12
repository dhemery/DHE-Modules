#include "./fixtures/controls-fixture.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {

using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;

class ControlsSuite : public Suite {
public:
  ControlsSuite() : Suite("dhe::curve_sequencer::Controls") {}
  void run(Tester &t) override {
    t.run("is_gated() is true if gate button is pressed",
          test([](Tester &t, Module &module, Controls &controls) {
            module.params_[Param::GateButton].setValue(1.F);

            t.assert_that(controls.is_gated(), is_true);
          }));

    t.run("is_gated() is true if gate input is high",
          test([](Tester &t, Module &module, Controls &controls) {
            module.inputs_[Input::GateInput].setVoltage(10.F);

            t.assert_that(controls.is_gated(), is_true);
          }));

    t.run("is_gated() is false :"
          "if gate input is low and gate button is not pressed",
          test([](Tester &t, Module &module, Controls &controls) {
            module.params_[Param ::GateButton].setValue(0.F);
            module.inputs_[Input::GateInput].setVoltage(0.F);

            t.assert_that(controls.is_gated(), is_false);
          }));

    t.run("is_looping() is true if loop button is pressed",
          test([](Tester &t, Module &module, Controls &controls) {
            module.params_[Param::LoopButton].setValue(1.F);

            t.assert_that(controls.is_looping(), is_true);
          }));

    t.run("is_looping() is true if loop input is high",
          test([](Tester &t, Module &module, Controls &controls) {
            module.inputs_[Input::LoopInput].setVoltage(10.F);

            t.assert_that(controls.is_looping(), is_true);
          }));

    t.run("is_looping() is false: "
          "if loop input is low and loop button is not pressed",
          test([](Tester &t, Module &module, Controls &controls) {
            module.params_[Param ::LoopButton].setValue(0.F);
            module.inputs_[Input::LoopInput].setVoltage(0.F);

            t.assert_that(controls.is_looping(), is_false);
          }));

    t.run("is_reset() is true if reset button is pressed",
          test([](Tester &t, Module &module, Controls &controls) {
            module.params_[Param::ResetButton].setValue(1.F);

            t.assert_that(controls.is_reset(), is_true);
          }));

    t.run("is_reset() is true if reset input is high",
          test([](Tester &t, Module &module, Controls &controls) {
            module.inputs_[Input::ResetInput].setVoltage(10.F);

            t.assert_that(controls.is_reset(), is_true);
          }));

    t.run("is_reset() is false: "
          "if reset input is low and reset button is not pressed",
          test([](Tester &t, Module &module, Controls &controls) {
            module.params_[Param ::ResetButton].setValue(0.F);
            module.inputs_[Input::ResetInput].setVoltage(0.F);

            t.assert_that(controls.is_reset(), is_false);
          }));

    t.run("is_running() is true if run button is pressed",
          test([](Tester &t, Module &module, Controls &controls) {
            module.params_[Param::RunButton].setValue(1.F);

            t.assert_that(controls.is_running(), is_true);
          }));

    t.run("is_running() is true if run input is high",
          test([](Tester &t, Module &module, Controls &controls) {
            module.inputs_[Input::RunInput].setVoltage(10.F);

            t.assert_that(controls.is_running(), is_true);
          }));

    t.run("is_running() is false: "
          "if run input is low and run button is not pressed",
          test([](Tester &t, Module &module, Controls &controls) {
            module.params_[Param ::RunButton].setValue(0.F);
            module.inputs_[Input::RunInput].setVoltage(0.F);

            t.assert_that(controls.is_running(), is_false);
          }));

    t.run("selection_start()",
          test([](Tester &t, Module &module, Controls &controls) {
            module.params_[Param::SelectionStartKnob].setValue(3.F);
            t.assert_that(controls.selection_start(), is_equal_to(3));
          }));

    t.run("selection_length()",
          test([](Tester &t, Module &module, Controls &controls) {
            module.params_[Param::SelectionLengthKnob].setValue(6.F);
            t.assert_that(controls.selection_length(), is_equal_to(6));
          }));

    t.run("input()", test([](Tester &t, Module &module, Controls &controls) {
            auto constexpr input_voltage = 3.1234F;
            module.inputs_[Input::CurveSequencerInput].setVoltage(
                input_voltage);
            t.assert_that(controls.input(), is_equal_to(input_voltage));
          }));

    t.run("output()", test([](Tester &t, Module &module, Controls &controls) {
            auto constexpr output_voltage = 9.13894F;
            module.outputs_[Output::CurveSequencerOutput].setVoltage(
                output_voltage);
            t.assert_that(controls.output(), is_equal_to(output_voltage));
          }));

    t.run("output(v)", test([](Tester &t, Module &module, Controls &controls) {
            auto constexpr output_voltage = 4.390984F;
            controls.output(output_voltage);
            t.assert_that(
                module.outputs_[Output::CurveSequencerOutput].getVoltage(),
                is_equal_to(output_voltage));
          }));
  }
};

static auto _ = ControlsSuite{};

} // namespace curve_sequencer
} // namespace test
