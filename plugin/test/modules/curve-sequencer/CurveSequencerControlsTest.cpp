#include "curve-sequencer/CurveSequencerControls.h"

#include "components/Taper.h"
#include "controls/CurvatureInputs.h"
#include "controls/DurationInputs.h"
#include "controls/LevelInputs.h"
#include "curve-sequencer/StageMode.h"

#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <gtest/gtest.h>

static auto constexpr stepCount{8};

using ::testing::Test;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;
using dhe::curve_sequencer::StageMode;

struct CurveSequencerControlsTest : public Test {
  std::vector<rack::engine::Input> inputs{Controls::InputCount};
  std::vector<rack::engine::Output> outputs{Controls::OutputCount};
  std::vector<rack::engine::Param> params{Controls::ParameterCount};
  std::vector<rack::engine::Light> lights{Controls::LightCount};

  Controls controls{inputs, outputs, params, lights};
};

TEST_F(CurveSequencerControlsTest, isGated_ifGateButtonIsPressed) {
  params[Controls::GateButton].setValue(1.F);

  ASSERT_TRUE(controls.isGated());
}

TEST_F(CurveSequencerControlsTest, isGated_ifGateInputIsHigh) {
  inputs[Controls::GateInput].setVoltage(10.F);

  ASSERT_TRUE(controls.isGated());
}

TEST_F(CurveSequencerControlsTest, isNotGated_ifGateButtonIsNotPressedAndGateInputIsLow) {
  ASSERT_FALSE(controls.isGated());
}

TEST_F(CurveSequencerControlsTest, isLooping_ifLoopButtonIsPressed) {
  params[Controls::LoopButton].setValue(1.F);

  ASSERT_TRUE(controls.isLooping());
}

TEST_F(CurveSequencerControlsTest, isLooping_ifLoopInputIsHigh) {
  inputs[Controls::LoopInput].setVoltage(10.F);

  ASSERT_TRUE(controls.isLooping());
}

TEST_F(CurveSequencerControlsTest, isNotLooping_ifLoopButtonIsNotPressedAndLoopInputIsLow) {
  ASSERT_FALSE(controls.isLooping());
}

TEST_F(CurveSequencerControlsTest, isReset_ifResetButtonIsPressed) {
  params[Controls::ResetButton].setValue(1.F);

  ASSERT_TRUE(controls.isReset());
}

TEST_F(CurveSequencerControlsTest, isReset_ifResetInputIsHigh) {
  inputs[Controls::ResetInput].setVoltage(10.F);

  ASSERT_TRUE(controls.isReset());
}

TEST_F(CurveSequencerControlsTest, isNotReset_ifResetButtonIsNotPressedAndResetInputIsLow) {
  ASSERT_FALSE(controls.isReset());
}

TEST_F(CurveSequencerControlsTest, isRunning_ifRunButtonIsPressed) {
  params[Controls::RunButton].setValue(1.F);

  ASSERT_TRUE(controls.isRunning());
}

TEST_F(CurveSequencerControlsTest, isRunning_ifRunInputIsHigh) {
  inputs[Controls::RunInput].setVoltage(10.F);

  ASSERT_TRUE(controls.isRunning());
}

TEST_F(CurveSequencerControlsTest, isNotRunning_ifRunButtonIsNotPressedAndRunInputIsLow) {
  ASSERT_FALSE(controls.isRunning());
}

TEST_F(CurveSequencerControlsTest, stepIsEnabled_ifStepEnabledButtonIsPressed) {
  auto constexpr step = 0;
  params[Controls::EnabledButtons + step].setValue(1.F);

  ASSERT_TRUE(controls.isEnabled(step));
}

TEST_F(CurveSequencerControlsTest, stepIsEnabled_ifStepEnabledInputIsHigh) {
  auto constexpr step = 7;
  inputs[Controls::EnabledInputs + step].setVoltage(10.F);

  ASSERT_TRUE(controls.isEnabled(step));
}

TEST_F(CurveSequencerControlsTest, stepIsNotEnabled_ifStepEnabledButtonIsNotPressedAndStepEnabledInputIsLow) {
  auto constexpr step = 2;
  ASSERT_FALSE(controls.isEnabled(step));
}

TEST_F(CurveSequencerControlsTest, selectionStart_reportsSelectionStartKnobValue) {
  params[Controls::SelectionStartKnob].setValue(3.F);

  ASSERT_EQ(controls.selectionStart(), 3);
}

TEST_F(CurveSequencerControlsTest, selectionLength_reportsSelectionLengthKnobValue) {
  params[Controls::SelectionLengthKnob].setValue(5.F);

  ASSERT_EQ(controls.selectionLength(), 5);
}

TEST_F(CurveSequencerControlsTest, input_reportsVoltageAtInputPort) {
  inputs[Controls::CurveSequencerInput].setVoltage(5.3F);

  ASSERT_EQ(controls.input(), 5.3F);
}

TEST_F(CurveSequencerControlsTest, output_setsVoltageAtOutputPort) {
  controls.output(5.3F);

  ASSERT_EQ(outputs[Controls::CurveSequencerInput].getVoltage(), 5.3F);
}

TEST_F(CurveSequencerControlsTest, output_reportsVoltageAtOutputPort) {
  outputs[Controls::CurveSequencerInput].setVoltage(7.1F);
  ASSERT_EQ(controls.output(), 7.1F);
}

TEST_F(CurveSequencerControlsTest, stepCurvature_reportsCurvatureForStepCurvatureParam) {
  auto constexpr step = 5;
  auto constexpr curveKnobRotation = 0.3F;
  params[Controls::CurveKnobs + step].setValue(curveKnobRotation);

  ASSERT_EQ(controls.curvature(step), dhe::curvature(curveKnobRotation));
}

TEST_F(CurveSequencerControlsTest, stepDuration_reportsDurationForDurationRangeSwitchAndStepDurationParam) {
  auto constexpr step = 7;
  auto constexpr durationKnobRotation = 0.75F;
  auto constexpr durationRangeSelection = 2; // Long duration

  params[Controls::DurationKnobs + step].setValue(durationKnobRotation);
  params[Controls::DurationRangeSwitch].setValue(static_cast<float>(durationRangeSelection));

  ASSERT_EQ(controls.duration(step), dhe::duration(durationKnobRotation, *dhe::durationRanges[durationRangeSelection]));
}

TEST_F(CurveSequencerControlsTest, stepLevel_reportsLevelForLevelRangeSwitchAndStepLevelParam) {
  auto constexpr step = 3;
  auto constexpr levelKnobRotation = 0.35F;
  auto constexpr levelRangeSelection = 1; // unipolar

  params[Controls::LevelKnobs + step].setValue(levelKnobRotation);
  params[Controls::LevelRangeSwitch].setValue(static_cast<float>(levelRangeSelection));

  ASSERT_EQ(controls.level(step), dhe::level(levelKnobRotation, *dhe::signalRanges[levelRangeSelection]));
}

TEST_F(CurveSequencerControlsTest, stepShape_isTaperSelectedByStepShapeSwitch) {
  auto constexpr step = 3;

  auto shapeSelection = 0; // J
  params[Controls::ShapeSwitches + step].setValue(static_cast<float>(shapeSelection));
  EXPECT_EQ(controls.taper(step), dhe::taper::variableTapers[shapeSelection]);

  shapeSelection = 1; // S
  params[Controls::ShapeSwitches + step].setValue(static_cast<float>(shapeSelection));
  EXPECT_EQ(controls.taper(step), dhe::taper::variableTapers[shapeSelection]);
}

TEST_F(CurveSequencerControlsTest, stepGenerateMode_isModeSelectedByStepGenerateModeStepper) {
  auto constexpr step = 6;

  auto modeSelectedByStepper = StageMode::High;
  params[Controls::GenerateModeSwitches + step].setValue(static_cast<float>(modeSelectedByStepper));
  EXPECT_EQ(controls.generateMode(step), modeSelectedByStepper);

  modeSelectedByStepper = StageMode::Low;
  params[Controls::GenerateModeSwitches + step].setValue(static_cast<float>(modeSelectedByStepper));
  EXPECT_EQ(controls.generateMode(step), modeSelectedByStepper);

  modeSelectedByStepper = StageMode::Calm;
  params[Controls::GenerateModeSwitches + step].setValue(static_cast<float>(modeSelectedByStepper));
  EXPECT_EQ(controls.generateMode(step), modeSelectedByStepper);

  modeSelectedByStepper = StageMode::Skip;
  params[Controls::GenerateModeSwitches + step].setValue(static_cast<float>(modeSelectedByStepper));
  EXPECT_EQ(controls.generateMode(step), modeSelectedByStepper);

  modeSelectedByStepper = StageMode::Generate;
  params[Controls::GenerateModeSwitches + step].setValue(static_cast<float>(modeSelectedByStepper));
  EXPECT_EQ(controls.generateMode(step), modeSelectedByStepper);
}

TEST_F(CurveSequencerControlsTest, stepSustainMode_isModeSelectedByStepSustainModeStepper) {
  auto constexpr step = 0;

  auto modeSelectedByStepper = StageMode::High;
  params[Controls::SustainModeSwitches + step].setValue(static_cast<float>(modeSelectedByStepper));
  EXPECT_EQ(controls.sustainMode(step), modeSelectedByStepper);

  modeSelectedByStepper = StageMode::Low;
  params[Controls::SustainModeSwitches + step].setValue(static_cast<float>(modeSelectedByStepper));
  EXPECT_EQ(controls.sustainMode(step), modeSelectedByStepper);

  modeSelectedByStepper = StageMode::Calm;
  params[Controls::SustainModeSwitches + step].setValue(static_cast<float>(modeSelectedByStepper));
  EXPECT_EQ(controls.sustainMode(step), modeSelectedByStepper);

  modeSelectedByStepper = StageMode::Skip;
  params[Controls::SustainModeSwitches + step].setValue(static_cast<float>(modeSelectedByStepper));
  EXPECT_EQ(controls.sustainMode(step), modeSelectedByStepper);
}

TEST_F(CurveSequencerControlsTest, showGenerating_setsStepGeneratingLightBrightness) {
  auto constexpr step = 3;

  controls.showGenerating(step, true);
  EXPECT_EQ(lights[Controls::GeneratingLights + step].getBrightness(), 10.F);

  controls.showGenerating(step, false);
  EXPECT_EQ(lights[Controls::GeneratingLights + step].getBrightness(), 0.F);
}

TEST_F(CurveSequencerControlsTest, showSustaining_setsStepSustainingLightBrightness) {
  auto constexpr step = 6;

  controls.showSustaining(step, true);
  EXPECT_EQ(lights[Controls::SustainingLights + step].getBrightness(), 10.F);

  controls.showSustaining(step, false);
  EXPECT_EQ(lights[Controls::SustainingLights + step].getBrightness(), 0.F);
}
