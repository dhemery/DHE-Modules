#include "curve-sequencer/Advancing.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

static auto constexpr stepCount{8};

using dhe::curve_sequencer::Advancing;
using dhe::curve_sequencer::SequenceMode;

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class MockControls {
public:
  MOCK_METHOD(int, selectionStart, (), (const));
  MOCK_METHOD(int, selectionLength, (), (const));
  MOCK_METHOD(bool, isEnabled, (int), (const));
};

struct AdvancingTest : public Test {
  NiceMock<MockControls> controls;
  Advancing<stepCount, MockControls> advancing{controls};

  void givenEnabled(int step, bool state) { ON_CALL(controls, isEnabled(step)).WillByDefault(Return(state)); }

  void givenSelection(int start, int length) {
    ON_CALL(controls, selectionStart()).WillByDefault(Return(start));
    ON_CALL(controls, selectionLength()).WillByDefault(Return(length));
  }
};

TEST_F(AdvancingTest, ifGivenStepIsEnabled_returnsGeneratingGivenStep) {
  givenSelection(0, stepCount); // [0 1 2 3 4 5 6 7]

  auto const givenStep = 2;
  givenEnabled(givenStep, true);

  auto const next = advancing.execute(givenStep);

  EXPECT_EQ(next.mode, SequenceMode::Generating);
  EXPECT_EQ(next.step, givenStep);
}

TEST_F(AdvancingTest, ifFirstEnabledStepInSelectionIsAboveGivenStep_returnsGeneratingEnabledStep) {
  givenSelection(0, stepCount); // [0 1 2 3 4 5 6 7]

  auto const givenStep = 2;
  auto const firstEnabledStep = givenStep + 2;
  givenEnabled(firstEnabledStep, true);

  auto const next = advancing.execute(givenStep);

  EXPECT_EQ(next.mode, SequenceMode::Generating);
  EXPECT_EQ(next.step, firstEnabledStep);
}

TEST_F(AdvancingTest, ifFirstEnabledStepIsLastStepInSelection_returnsGeneratingEnabledStep) {
  auto const selectionStart = 2;
  auto const selectionLength = 3;
  givenSelection(selectionStart, selectionLength); // [2 3 4]

  auto const lastStepInSelection = selectionStart + selectionLength - 1;
  givenEnabled(lastStepInSelection, true);

  auto const next = advancing.execute(selectionStart);

  EXPECT_EQ(next.mode, SequenceMode::Generating);
  EXPECT_EQ(next.step, lastStepInSelection);
}

TEST_F(AdvancingTest, ifNoStepInSelectionIsEnabled_returnsIdle) {
  auto const selectionStart = 1;
  auto const selectionLength = 4;
  givenSelection(selectionStart, selectionLength); // [1 2 3 4]

  givenEnabled(selectionStart + selectionLength, true); // Enabled but not in selection

  auto const next = advancing.execute(selectionStart);

  EXPECT_EQ(next.mode, SequenceMode::Idle);
}

TEST_F(AdvancingTest, ifFirstEnabledStepInWrappedSelectionIsBelowSelectionStart_returnsGeneratingEnabledStep) {
  auto const selectionStart = 6;
  auto const selectionLength = 4;
  givenSelection(selectionStart, selectionLength); // [6 7 0 1]

  auto const enabledSelectedStepBelowSelectionStart = 1; // Must wrap to find
  givenEnabled(enabledSelectedStepBelowSelectionStart, true);

  auto const next = advancing.execute(selectionStart);

  EXPECT_EQ(next.mode, SequenceMode::Generating);
  EXPECT_EQ(next.step, enabledSelectedStepBelowSelectionStart);
}

TEST_F(AdvancingTest, ifNoStepInWrappedSelectionIsEnabled_returnsIdle) {
  auto const selectionStart = 5;
  auto const selectionLength = 7;
  givenSelection(selectionStart, selectionLength); // [5 6 7 0 1 2 3]

  auto const unselectedStep = 4; // The only enabled step is not in the selection
  givenEnabled(unselectedStep, true);

  auto const next = advancing.execute(selectionStart);

  EXPECT_EQ(next.mode, SequenceMode::Idle);
}

TEST_F(AdvancingTest, ifGivenStepIsBelowSelection_returnsIdle) {
  givenSelection(4, 3); // [4 5 6]

  auto const givenStep = 1; // Lower than any selected step
  givenEnabled(givenStep, true);

  auto const next = advancing.execute(givenStep);

  EXPECT_EQ(next.mode, SequenceMode::Idle);
}

TEST_F(AdvancingTest, ifGivenStepIsAboveSelection_returnsIdle) {
  givenSelection(4, 3); // [4 5 6]

  auto const givenStep = 7; // Higher than any selected step
  givenEnabled(givenStep, true);

  auto const next = advancing.execute(givenStep);

  EXPECT_EQ(next.mode, SequenceMode::Idle);
}

TEST_F(AdvancingTest, ifGivenStepIsBetweenEndpointsOfWrappedSelection_returnsIdle) {
  givenSelection(6, 4); // [6 7 0 1]

  auto const givenStep = 5; // Above selection end, below selection start
  givenEnabled(givenStep, true);

  auto const next = advancing.execute(givenStep);

  EXPECT_EQ(next.mode, SequenceMode::Idle);
}

TEST_F(AdvancingTest, ifGivenStepIsEnabledAndSelectedAndBelowSelectionStart_returnsGeneratingGivenStep) {
  auto const selectionStart = 6;
  auto const selectionLength = 6;
  givenSelection(selectionStart, selectionLength); // [6 7 0 1 2 3]

  auto const enabledSelectedStepBelowSelectionStart = 2; // Must wrap to find
  givenEnabled(enabledSelectedStepBelowSelectionStart, true);

  auto const next = advancing.execute(enabledSelectedStepBelowSelectionStart);

  EXPECT_EQ(next.mode, SequenceMode::Generating);
}
