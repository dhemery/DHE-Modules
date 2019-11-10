#include "curve-sequencer/Advancing.h"

#include "curve-sequencer/CurveSequencerControls.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <gtest/gtest.h>

static auto constexpr stepCount{8};

using dhe::curve_sequencer::Advancing;
using dhe::curve_sequencer::SequenceMode;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;

using ::testing::Test;

struct AdvancingTest : public Test {
  std::vector<rack::engine::Input> inputs{Controls::InputCount};
  std::vector<rack::engine::Param> params{Controls::ParameterCount};

  Advancing<stepCount> advancing{inputs, params};

  void givenEnabledButton(int step, bool state) { params[Controls::EnabledButtons + step].setValue(state ? 1.F : 0.F); }
  void givenEnabledInput(int step, bool state) { inputs[Controls::EnabledInputs + step].setVoltage(state ? 10.F : 0.F); }
  void givenSelection(int start, int length) {
    params[Controls::SelectionStartKnob].setValue(static_cast<float>(start));
    params[Controls::SelectionLengthKnob].setValue(static_cast<float>(length));
  }
};

TEST_F(AdvancingTest, detectsStepEnabledByButton) {
  auto const selectionStart = 0;
  auto const selectionLength = stepCount;
  givenSelection(selectionStart, selectionLength); // [0 1 2 3 4 5 6 7]

  auto const enabledStep = 4;
  givenEnabledButton(enabledStep, true);

  auto const next = advancing.execute(selectionStart);

  ASSERT_EQ(next.step, enabledStep);
}

TEST_F(AdvancingTest, detectsStepEnabledByInput) {
  auto const selectionStart = 0;
  givenSelection(selectionStart, stepCount); // [0 1 2 3 4 5 6 7]

  auto const enabledStep = 4;
  givenEnabledInput(enabledStep, true);

  auto const next = advancing.execute(selectionStart);

  ASSERT_EQ(next.step, enabledStep);
}

TEST_F(AdvancingTest, ifGivenStepIsEnabled_returnsGeneratingGivenStep) {
  givenSelection(0, stepCount); // [0 1 2 3 4 5 6 7]

  auto const givenStep = 2;
  givenEnabledButton(givenStep, true);

  auto const next = advancing.execute(givenStep);

  EXPECT_EQ(next.mode, SequenceMode::Generating);
  EXPECT_EQ(next.step, givenStep);
}

TEST_F(AdvancingTest, ifFirstEnabledStepInSelectionIsAboveGivenStep_returnsGeneratingEnabledStep) {
  givenSelection(0, stepCount); // [0 1 2 3 4 5 6 7]

  auto const givenStep = 2;
  auto const firstEnabledStep = givenStep + 2;
  givenEnabledButton(firstEnabledStep, true);

  auto const next = advancing.execute(givenStep);

  EXPECT_EQ(next.mode, SequenceMode::Generating);
  EXPECT_EQ(next.step, firstEnabledStep);
}

TEST_F(AdvancingTest, ifFirstEnabledStepIsLastStepInSelection_returnsGeneratingEnabledStep) {
  auto const selectionStart = 2;
  auto const selectionLength = 3;
  givenSelection(selectionStart, selectionLength); // [2 3 4]

  auto const lastStepInSelection = selectionStart + selectionLength - 1;
  givenEnabledButton(lastStepInSelection, true);

  auto const next = advancing.execute(selectionStart);

  EXPECT_EQ(next.mode, SequenceMode::Generating);
  EXPECT_EQ(next.step, lastStepInSelection);
}

TEST_F(AdvancingTest, ifNoStepInSelectionIsEnabled_returnsIdle) {
  auto const selectionStart = 1;
  auto const selectionLength = 4;
  givenSelection(selectionStart, selectionLength); // [1 2 3 4]

  givenEnabledButton(selectionStart + selectionLength, true); // Enabled but not in selection

  auto const next = advancing.execute(selectionStart);

  EXPECT_EQ(next.mode, SequenceMode::Idle);
}

TEST_F(AdvancingTest, ifFirstEnabledStepInWrappedSelectionIsBelowSelectionStart_returnsGeneratingEnabledStep) {
  auto const selectionStart = 6;
  auto const selectionLength = 4;
  givenSelection(selectionStart, selectionLength); // [6 7 0 1]

  auto const enabledSelectedStepBelowSelectionStart = 1; // Must wrap to find
  givenEnabledButton(enabledSelectedStepBelowSelectionStart, true);

  auto const next = advancing.execute(selectionStart);

  EXPECT_EQ(next.mode, SequenceMode::Generating);
  EXPECT_EQ(next.step, enabledSelectedStepBelowSelectionStart);
}

TEST_F(AdvancingTest, ifNoStepInWrappedSelectionIsEnabled_returnsIdle) {
  auto const selectionStart = 5;
  auto const selectionLength = 7;
  givenSelection(selectionStart, selectionLength); // [5 6 7 0 1 2 3]

  auto const unselectedStep = 4; // The only enabled step is not in the selection
  givenEnabledButton(unselectedStep, true);

  auto const next = advancing.execute(selectionStart);

  EXPECT_EQ(next.mode, SequenceMode::Idle);
}

TEST_F(AdvancingTest, ifGivenStepIsBelowSelection_returnsIdle) {
  givenSelection(4, 3); // [4 5 6]

  auto const givenStep = 1; // Lower than any selected step
  givenEnabledButton(givenStep, true);

  auto const next = advancing.execute(givenStep);

  EXPECT_EQ(next.mode, SequenceMode::Idle);
}

TEST_F(AdvancingTest, ifGivenStepIsAboveSelection_returnsIdle) {
  givenSelection(4, 3); // [4 5 6]

  auto const givenStep = 7; // Higher than any selected step
  givenEnabledButton(givenStep, true);

  auto const next = advancing.execute(givenStep);

  EXPECT_EQ(next.mode, SequenceMode::Idle);
}

TEST_F(AdvancingTest, ifGivenStepIsBetweenEndpointsOfWrappedSelection_returnsIdle) {
  givenSelection(6, 4); // [6 7 0 1]

  auto const givenStep = 5; // Above selection end, below selection start
  givenEnabledButton(givenStep, true);

  auto const next = advancing.execute(givenStep);

  EXPECT_EQ(next.mode, SequenceMode::Idle);
}

TEST_F(AdvancingTest, ifGivenStepIsEnabledAndSelectedAndBelowSelectionStart_returnsGeneratingGivenStep) {
  auto const selectionStart = 6;
  auto const selectionLength = 6;
  givenSelection(selectionStart, selectionLength); // [6 7 0 1 2 3]

  auto const enabledSelectedStepBelowSelectionStart = 2; // Must wrap to find
  givenEnabledButton(enabledSelectedStepBelowSelectionStart, true);

  auto const next = advancing.execute(enabledSelectedStepBelowSelectionStart);

  EXPECT_EQ(next.mode, SequenceMode::Generating);
}
