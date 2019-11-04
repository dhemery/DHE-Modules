#include "curve-sequencer/AdvancingMode.h"

#include "curve-sequencer/CurveSequencerControls.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <gtest/gtest.h>

static auto constexpr stepCount{8};

using dhe::curve_sequencer::AdvancingMode;
using dhe::curve_sequencer::ModeId;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;

using ::testing::Test;

struct AdvancingModeTest : public Test {
  std::vector<rack::engine::Input> inputs{Controls::InputCount};
  std::vector<rack::engine::Param> params{Controls::ParameterCount};

  AdvancingMode<stepCount> advancingMode{inputs, params};

  void setEnabledButton(int step, bool state) { params[Controls::EnabledButtons + step].setValue(state ? 1.F : 0.F); }
  void setEnabledInput(int step, bool state) { inputs[Controls::EnabledInputs + step].setVoltage(state ? 10.F : 0.F); }
  void setSelection(int start, int length) {
    params[Controls::StartKnob].setValue(static_cast<float>(start));
    params[Controls::StepsKnob].setValue(static_cast<float>(length));
  }
};

TEST_F(AdvancingModeTest, detectsStepEnabledByButton) {
  auto const selectionStart = 0;
  auto const selectionLength = stepCount;
  setSelection(selectionStart, selectionLength); // [0 1 2 3 4 5 6 7]

  auto const enabledStep = 4;
  setEnabledButton(enabledStep, true);

  auto const next = advancingMode.execute(selectionStart);

  ASSERT_EQ(next.step, enabledStep);
}

TEST_F(AdvancingModeTest, detectsStepEnabledByInput) {
  auto const selectionStart = 0;
  setSelection(selectionStart, stepCount); // [0 1 2 3 4 5 6 7]

  auto const enabledStep = 4;
  setEnabledInput(enabledStep, true);

  auto const next = advancingMode.execute(selectionStart);

  ASSERT_EQ(next.step, enabledStep);
}

TEST_F(AdvancingModeTest, ifGivenStepIsEnabled_returnsGeneratingGivenStep) {
  setSelection(0, stepCount); // [0 1 2 3 4 5 6 7]

  auto const givenStep = 2;
  setEnabledButton(givenStep, true);

  auto const next = advancingMode.execute(givenStep);

  EXPECT_EQ(next.mode, ModeId::Generating);
  EXPECT_EQ(next.step, givenStep);
}

TEST_F(AdvancingModeTest, ifFirstEnabledStepInSelectionIsAboveGivenStep_returnsGeneratingEnabledStep) {
  setSelection(0, stepCount); // [0 1 2 3 4 5 6 7]

  auto const givenStep = 2;
  auto const firstEnabledStep = givenStep + 2;
  setEnabledButton(firstEnabledStep, true);

  auto const next = advancingMode.execute(givenStep);

  EXPECT_EQ(next.mode, ModeId::Generating);
  EXPECT_EQ(next.step, firstEnabledStep);
}

TEST_F(AdvancingModeTest, ifFirstEnabledStepIsLastStepInSelection_returnsGeneratingEnabledStep) {
  auto const selectionStart = 2;
  auto const selectionLength = 3;
  setSelection(selectionStart, selectionLength); // [2 3 4]

  auto const lastStepInSelection = selectionStart + selectionLength - 1;
  setEnabledButton(lastStepInSelection, true);

  auto const next = advancingMode.execute(selectionStart);

  EXPECT_EQ(next.mode, ModeId::Generating);
  EXPECT_EQ(next.step, lastStepInSelection);
}

TEST_F(AdvancingModeTest, ifNoStepInSelectionIsEnabled_returnsIdle) {
  auto const selectionStart = 1;
  auto const selectionLength = 4;
  setSelection(selectionStart, selectionLength); // [1 2 3 4]

  setEnabledButton(selectionStart + selectionLength, true); // Enabled but not in selection

  auto const next = advancingMode.execute(selectionStart);

  EXPECT_EQ(next.mode, ModeId::Idle);
}

TEST_F(AdvancingModeTest, ifFirstEnabledStepInWrappedSelectionIsBelowSelectionStart_returnsGeneratingEnabledStep) {
  auto const selectionStart = 6;
  auto const selectionLength = 4;
  setSelection(selectionStart, selectionLength); // [6 7 0 1]

  auto const enabledSelectedStepBelowSelectionStart = 1; // Must wrap to find
  setEnabledButton(enabledSelectedStepBelowSelectionStart, true);

  auto const next = advancingMode.execute(selectionStart);

  EXPECT_EQ(next.mode, ModeId::Generating);
  EXPECT_EQ(next.step, enabledSelectedStepBelowSelectionStart);
}

TEST_F(AdvancingModeTest, ifNoStepInWrappedSelectionIsEnabled_returnsIdle) {
  auto const selectionStart = 5;
  auto const selectionLength = 7;
  setSelection(selectionStart, selectionLength); // [5 6 7 0 1 2 3]

  auto const unselectedStep = 4; // The only enabled step is not in the selection
  setEnabledButton(unselectedStep, true);

  auto const next = advancingMode.execute(selectionStart);

  EXPECT_EQ(next.mode, ModeId::Idle);
}

TEST_F(AdvancingModeTest, ifGivenStepIsBelowSelection_returnsIdle) {
  setSelection(4, 3); // [4 5 6]

  auto const givenStep = 1; // Lower than any selected step
  setEnabledButton(givenStep, true);

  auto const next = advancingMode.execute(givenStep);

  EXPECT_EQ(next.mode, ModeId::Idle);
}

TEST_F(AdvancingModeTest, ifGivenStepIsAboveSelection_returnsIdle) {
  setSelection(4, 3); // [4 5 6]

  auto const givenStep = 7; // Higher than any selected step
  setEnabledButton(givenStep, true);

  auto const next = advancingMode.execute(givenStep);

  EXPECT_EQ(next.mode, ModeId::Idle);
}

TEST_F(AdvancingModeTest, ifGivenStepIsBetweenEndpointsOfWrappedSelection_returnsIdle) {
  setSelection(6, 4); // [6 7 0 1]

  auto const givenStep = 5; // Above selection end, below selection start
  setEnabledButton(givenStep, true);

  auto const next = advancingMode.execute(givenStep);

  EXPECT_EQ(next.mode, ModeId::Idle);
}

TEST_F(AdvancingModeTest, ifGivenStepIsEnabledAndSelectedAndBelowSelectionStart_returnsGeneratingGivenStep) {
  auto const selectionStart = 6;
  auto const selectionLength = 6;
  setSelection(selectionStart, selectionLength); // [6 7 0 1 2 3]

  auto const enabledSelectedStepBelowSelectionStart = 2; // Must wrap to find
  setEnabledButton(enabledSelectedStepBelowSelectionStart, true);

  auto const next = advancingMode.execute(enabledSelectedStepBelowSelectionStart);

  EXPECT_EQ(next.mode, ModeId::Generating);
}
