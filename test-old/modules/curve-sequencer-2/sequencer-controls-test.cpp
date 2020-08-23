#include "fake-rack/Controls.h"
#include "modules/curve-sequencer-2/controls.h"

#include <doctest.h>

namespace test {
namespace curve_sequencer_2 {
namespace sequence_controls {
TEST_CASE("curve_sequencer_2::SequenceControls") {
  static auto constexpr step_count{8};

  using Controls = dhe::curve_sequencer_2::Controls<fake::Port, fake::Param,
                                                    fake::Light, step_count>;
  using dhe::curve_sequencer_2::TriggerMode;

  std::vector<fake::Port> inputs{Controls::InputCount};
  std::vector<fake::Port> outputs{Controls::OutputCount};
  std::vector<fake::Param> params{Controls::ParameterCount};
  std::vector<fake::Light> lights{Controls::LightCount};

  Controls controls{inputs, outputs, params, lights};

  SUBCASE("selectionStart() reports selection start knob value") {
    params[Controls::SelectionStartKnob].setValue(3.F);

    CHECK_EQ(controls.selection_start(), 3);
  }

  SUBCASE("selectionLength() reports selection length knob value") {
    params[Controls::SelectionLengthKnob].setValue(5.F);

    CHECK_EQ(controls.selection_length(), 5);
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
} // namespace curve_sequencer_2
} // namespace test
