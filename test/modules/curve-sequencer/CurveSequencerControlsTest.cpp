#include "modules/curve-sequencer/CurveSequencerControls.h"

#include "components/Taper.h"
#include "controls/CurvatureInputs.h"
#include "controls/DurationInputs.h"
#include "controls/LevelInputs.h"
#include "modules/curve-sequencer/GenerateMode.h"

#define DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES
#include "doctest/doctest.h"

static auto constexpr stepCount{8};

using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;
using dhe::curve_sequencer::AdvanceMode;
using dhe::curve_sequencer::GenerateMode;

DOCTEST_TEST_SUITE("CurveSequencerControls") {
  std::vector<rack::engine::Input> inputs{Controls::InputCount};
  std::vector<rack::engine::Output> outputs{Controls::OutputCount};
  std::vector<rack::engine::Param> params{Controls::ParameterCount};
  std::vector<rack::engine::Light> lights{Controls::LightCount};

  Controls controls{inputs, outputs, params, lights};

  DOCTEST_TEST_CASE("isGated()") {
    DOCTEST_SUBCASE("true if gate button is pressed") {
      params[Controls::GateButton].setValue(1.F);

      DOCTEST_CHECK(controls.isGated());
    }

    DOCTEST_SUBCASE("true if gate input is high") {
      inputs[Controls::GateInput].setVoltage(10.F);

      DOCTEST_CHECK(controls.isGated());
    }

    DOCTEST_SUBCASE("true if gate button is not pressed and gate input is low") {
      params[Controls::GateButton].setValue(0.F);
      inputs[Controls::GateInput].setVoltage(0.F);

      DOCTEST_CHECK_FALSE(controls.isGated());
    }
  }

  DOCTEST_TEST_CASE("isLooping()") {
    DOCTEST_SUBCASE("true if loop button is pressed") {
      params[Controls::LoopButton].setValue(1.F);

      DOCTEST_CHECK(controls.isLooping());
    }

    DOCTEST_SUBCASE("true if loop input is high") {
      inputs[Controls::LoopInput].setVoltage(10.F);

      DOCTEST_CHECK(controls.isLooping());
    }

    DOCTEST_SUBCASE("true if loop button is not pressed and loop input is low") {
      params[Controls::LoopButton].setValue(0.F);
      inputs[Controls::LoopInput].setVoltage(0.F);

      DOCTEST_CHECK_FALSE(controls.isLooping());
    }
  }

  DOCTEST_TEST_CASE("isReset()") {
    DOCTEST_SUBCASE("true if reset button is pressed") {
      params[Controls::ResetButton].setValue(1.F);

      DOCTEST_CHECK(controls.isReset());
    }

    DOCTEST_SUBCASE("true if reset input is high") {
      inputs[Controls::ResetInput].setVoltage(10.F);

      DOCTEST_CHECK(controls.isReset());
    }

    DOCTEST_SUBCASE("true if reset button is not pressed and reset input is low") {
      params[Controls::ResetButton].setValue(0.F);
      inputs[Controls::ResetInput].setVoltage(0.F);

      DOCTEST_CHECK_FALSE(controls.isReset());
    }
  }

  DOCTEST_TEST_CASE("isRunning()") {
    DOCTEST_SUBCASE("true if run button is pressed") {
      params[Controls::RunButton].setValue(1.F);

      DOCTEST_CHECK(controls.isRunning());
    }

    DOCTEST_SUBCASE("true if run input is high") {
      inputs[Controls::RunInput].setVoltage(10.F);

      DOCTEST_CHECK(controls.isRunning());
    }

    DOCTEST_SUBCASE("true if run button is not pressed and run input is low") {
      params[Controls::RunButton].setValue(0.F);
      inputs[Controls::RunInput].setVoltage(0.F);

      DOCTEST_CHECK_FALSE(controls.isRunning());
    }
  }

  DOCTEST_TEST_CASE("isEnabled(step)") {
    DOCTEST_SUBCASE("true if enabled button is pressed") {
      auto constexpr step = 3;
      params[Controls::EnabledButtons + step].setValue(1.F);

      DOCTEST_CHECK(controls.isEnabled(step));
    }

    DOCTEST_SUBCASE("true if enabled input is high") {
      auto constexpr step = 6;
      inputs[Controls::EnabledInputs + step].setVoltage(10.F);

      DOCTEST_CHECK(controls.isEnabled(step));
    }

    DOCTEST_SUBCASE("true if enabled button is not pressed and enabled input is low") {
      auto constexpr step = 7;

      params[Controls::EnabledButtons + step].setValue(0.F);
      inputs[Controls::EnabledInputs + step].setVoltage(0.F);

      DOCTEST_CHECK_FALSE(controls.isEnabled(step));
    }
  }

  DOCTEST_TEST_CASE("selectionStart() reports selection start knob value") {
    params[Controls::SelectionStartKnob].setValue(3.F);

    DOCTEST_CHECK_EQ(controls.selectionStart(), 3);
  }

  DOCTEST_TEST_CASE("selectionLength() reports selection length knob value") {
    params[Controls::SelectionLengthKnob].setValue(5.F);

    DOCTEST_CHECK_EQ(controls.selectionLength(), 5);
  }

  DOCTEST_TEST_CASE("input() reports voltage at input port") {
    inputs[Controls::CurveSequencerInput].setVoltage(5.3F);

    DOCTEST_CHECK_EQ(controls.input(), 5.3F);
  }

  DOCTEST_TEST_CASE("output() reports voltage at output port") {
    outputs[Controls::CurveSequencerOutput].setVoltage(7.1F);

    DOCTEST_CHECK_EQ(controls.output(), 7.1F);
  }

  DOCTEST_TEST_CASE("output(voltage) sets voltage at output port") {
    controls.output(5.3F);

    DOCTEST_CHECK_EQ(outputs[Controls::CurveSequencerOutput].getVoltage(), 5.3F);
  }

  DOCTEST_TEST_CASE("curvature(step)  reports curvature for step curvature param") {
    auto constexpr step = 5;
    auto constexpr curveKnobRotation = 0.3F;
    params[Controls::CurveKnobs + step].setValue(curveKnobRotation);

    DOCTEST_CHECK_EQ(controls.curvature(step), dhe::curvature(curveKnobRotation));
  }

  DOCTEST_TEST_CASE("duration(step) reports duration for duration range switch and step duration param") {
    auto constexpr step = 7;
    auto constexpr durationKnobRotation = 0.75F;
    auto constexpr durationRangeSelection = 2; // Long duration

    params[Controls::DurationKnobs + step].setValue(durationKnobRotation);
    params[Controls::DurationRangeSwitch].setValue(static_cast<float>(durationRangeSelection));

    DOCTEST_CHECK_EQ(controls.duration(step),
                     dhe::duration(durationKnobRotation, *dhe::durationRanges[durationRangeSelection]));
  }

  DOCTEST_TEST_CASE("level(step) reports level for level range switch and step level param") {
    auto constexpr step = 3;
    auto constexpr levelKnobRotation = 0.35F;
    auto constexpr levelRangeSelection = 1; // unipolar

    params[Controls::LevelKnobs + step].setValue(levelKnobRotation);
    params[Controls::LevelRangeSwitch].setValue(static_cast<float>(levelRangeSelection));

    DOCTEST_CHECK_EQ(controls.level(step), dhe::level(levelKnobRotation, *dhe::signalRanges[levelRangeSelection]));
  }

  DOCTEST_TEST_CASE("taper(step) is taper selected by step shape switch") {
    auto constexpr step = 3;

    auto shapeSelection = 0; // J
    params[Controls::ShapeSwitches + step].setValue(static_cast<float>(shapeSelection));
    DOCTEST_CHECK_EQ(controls.taper(step), dhe::taper::variableTapers[shapeSelection]);

    shapeSelection = 1; // S
    params[Controls::ShapeSwitches + step].setValue(static_cast<float>(shapeSelection));
    DOCTEST_CHECK_EQ(controls.taper(step), dhe::taper::variableTapers[shapeSelection]);
  }

  DOCTEST_TEST_CASE("mode(step) is mode selected by step mode switch") {
    auto constexpr step = 6;

    auto modeSelectedBySwitch = GenerateMode::Curve;
    params[Controls::ModeSwitches + step].setValue(static_cast<float>(modeSelectedBySwitch));
    DOCTEST_CHECK_EQ(controls.mode(step), modeSelectedBySwitch);

    modeSelectedBySwitch = GenerateMode::Hold;
    params[Controls::ModeSwitches + step].setValue(static_cast<float>(modeSelectedBySwitch));
    DOCTEST_CHECK_EQ(controls.mode(step), modeSelectedBySwitch);
  }

  DOCTEST_TEST_CASE("condition(step) is condition selected by step condition switch") {
    auto constexpr step = 0;

    auto conditionSelectedBySwitch = AdvanceMode::TimerExpires;
    params[Controls::ConditionSwitches + step].setValue(static_cast<float>(conditionSelectedBySwitch));
    DOCTEST_CHECK_EQ(controls.condition(step), conditionSelectedBySwitch);

    conditionSelectedBySwitch = AdvanceMode::GateIsHigh;
    params[Controls::ConditionSwitches + step].setValue(static_cast<float>(conditionSelectedBySwitch));
    DOCTEST_CHECK_EQ(controls.condition(step), conditionSelectedBySwitch);

    conditionSelectedBySwitch = AdvanceMode::GateIsLow;
    params[Controls::ConditionSwitches + step].setValue(static_cast<float>(conditionSelectedBySwitch));
    DOCTEST_CHECK_EQ(controls.condition(step), conditionSelectedBySwitch);

    conditionSelectedBySwitch = AdvanceMode::GateRises;
    params[Controls::ConditionSwitches + step].setValue(static_cast<float>(conditionSelectedBySwitch));
    DOCTEST_CHECK_EQ(controls.condition(step), conditionSelectedBySwitch);

    conditionSelectedBySwitch = AdvanceMode::GateFalls;
    params[Controls::ConditionSwitches + step].setValue(static_cast<float>(conditionSelectedBySwitch));
    DOCTEST_CHECK_EQ(controls.condition(step), conditionSelectedBySwitch);

    conditionSelectedBySwitch = AdvanceMode::GateChanges;
    params[Controls::ConditionSwitches + step].setValue(static_cast<float>(conditionSelectedBySwitch));
    DOCTEST_CHECK_EQ(controls.condition(step), conditionSelectedBySwitch);
  }

  DOCTEST_TEST_CASE("showInactive(step) dims step progress lights") {
    auto constexpr step = 3;
    auto constexpr completedProgressLightIndex = step + step;
    auto constexpr remainingProgressLightIndex = step + step + 1;

    controls.showInactive(step);

    DOCTEST_CHECK_EQ(lights[Controls::ProgressLights + completedProgressLightIndex].getBrightness(), 0.F);
    DOCTEST_CHECK_EQ(lights[Controls::ProgressLights + remainingProgressLightIndex].getBrightness(), 0.F);
  }
}
