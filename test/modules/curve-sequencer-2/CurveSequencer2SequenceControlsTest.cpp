#include "doctest/doctest.h"
#include "fake-rack/Controls.h"
#include "modules/curve-sequencer-2/CurveSequencer2Controls.h"

namespace test {
namespace curve_sequencer_2 {
  namespace sequence_controls {
    TEST_CASE("curve_sequencer::CurveSequencer2SequenceControls") {
      static auto constexpr stepCount{8};

      using Controls = dhe::curve_sequencer::CurveSequencer2Controls<fake::Port, fake::Param, fake::Light, stepCount>;
      using dhe::curve_sequencer::TriggerMode;

      std::vector<fake::Port> inputs{Controls::InputCount};
      std::vector<fake::Port> outputs{Controls::OutputCount};
      std::vector<fake::Param> params{Controls::ParameterCount};
      std::vector<fake::Light> lights{Controls::LightCount};

      Controls controls{inputs, outputs, params, lights};

      SUBCASE("isGated()") {
        SUBCASE("true if gate button is pressed") {
          params[Controls::GateButton].setValue(1.F);

          CHECK(controls.isGated());
        }

        SUBCASE("true if gate input is high") {
          inputs[Controls::GateInput].setVoltage(10.F);

          CHECK(controls.isGated());
        }

        SUBCASE("true if gate button is not pressed and gate input is low") {
          params[Controls::GateButton].setValue(0.F);
          inputs[Controls::GateInput].setVoltage(0.F);

          CHECK_FALSE(controls.isGated());
        }
      }

      SUBCASE("isLooping()") {
        SUBCASE("true if loop button is pressed") {
          params[Controls::LoopButton].setValue(1.F);

          CHECK(controls.isLooping());
        }

        SUBCASE("true if loop input is high") {
          inputs[Controls::LoopInput].setVoltage(10.F);

          CHECK(controls.isLooping());
        }

        SUBCASE("true if loop button is not pressed and loop input is low") {
          params[Controls::LoopButton].setValue(0.F);
          inputs[Controls::LoopInput].setVoltage(0.F);

          CHECK_FALSE(controls.isLooping());
        }
      }

      SUBCASE("isReset()") {
        SUBCASE("true if reset button is pressed") {
          params[Controls::ResetButton].setValue(1.F);

          CHECK(controls.isReset());
        }

        SUBCASE("true if reset input is high") {
          inputs[Controls::ResetInput].setVoltage(10.F);

          CHECK(controls.isReset());
        }

        SUBCASE("true if reset button is not pressed and reset input is low") {
          params[Controls::ResetButton].setValue(0.F);
          inputs[Controls::ResetInput].setVoltage(0.F);

          CHECK_FALSE(controls.isReset());
        }
      }

      SUBCASE("isRunning()") {
        SUBCASE("true if run button is pressed") {
          params[Controls::RunButton].setValue(1.F);

          CHECK(controls.isRunning());
        }

        SUBCASE("true if run input is high") {
          inputs[Controls::RunInput].setVoltage(10.F);

          CHECK(controls.isRunning());
        }

        SUBCASE("true if run button is not pressed and run input is low") {
          params[Controls::RunButton].setValue(0.F);
          inputs[Controls::RunInput].setVoltage(0.F);

          CHECK_FALSE(controls.isRunning());
        }
      }

      SUBCASE("selectionStart() reports selection start knob value") {
        params[Controls::SelectionStartKnob].setValue(3.F);

        CHECK_EQ(controls.selectionStart(), 3);
      }

      SUBCASE("selectionLength() reports selection length knob value") {
        params[Controls::SelectionLengthKnob].setValue(5.F);

        CHECK_EQ(controls.selectionLength(), 5);
      }

      SUBCASE("input() reports voltage at input port") {
        inputs[Controls::CurveSequencerInput].setVoltage(5.3F);

        CHECK_EQ(controls.input(), 5.3F);
      }

      SUBCASE("output() reports voltage at output port") {
        outputs[Controls::CurveSequencerOutput].setVoltage(7.1F);

        CHECK_EQ(controls.output(), 7.1F);
      }

      SUBCASE("output(voltage) sets voltage at output port") {
        controls.output(5.3F);

        CHECK_EQ(outputs[Controls::CurveSequencerOutput].getVoltage(), 5.3F);
      }
    }
  } // namespace sequence_controls
} // namespace curve_sequencer
} // namespace test
