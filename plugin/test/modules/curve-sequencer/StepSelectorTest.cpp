#include "curve-sequencer/StepSelector.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::curve_sequencer::StepSelector;

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class MockControls {
public:
  MOCK_METHOD(int, selectionStart, (), (const));
  MOCK_METHOD(int, selectionLength, (), (const));
  MOCK_METHOD(bool, isEnabled, (int), (const));
};

struct StepSelectorTest : public Test {
  static auto constexpr stepCount{8};
  NiceMock<MockControls> controls;
  StepSelector<MockControls> selector{controls, stepCount};

  void givenEnabled(int step, bool state) { ON_CALL(controls, isEnabled(step)).WillByDefault(Return(state)); }

  void givenSelection(int start, int length) {
    ON_CALL(controls, selectionStart()).WillByDefault(Return(start));
    ON_CALL(controls, selectionLength()).WillByDefault(Return(length));
  }
};

TEST_F(StepSelectorTest, successor_ifGivenStepIsSelectionEnd_isNegative) {
  auto constexpr step = 3;
  givenSelection(1, step); // 0 [1 2 3] 4 5 6 7

  ASSERT_LT(selector.successor(step), 0);
}

TEST_F(StepSelectorTest, successor_ifFirstEnabledSuccessorIsAboveGivenStep_returnsEnabledStep) {
  givenSelection(0, stepCount); // [0 1 2 3 4 5 6 7]

  auto const givenStep = 2;
  auto const firstEnabledStep = givenStep + 2;
  givenEnabled(firstEnabledStep, true);

  ASSERT_EQ(selector.successor(givenStep), firstEnabledStep);
}

TEST_F(StepSelectorTest, successor_ifFirstEnabledSuccessorIsSelectionEnd_returnsEnabledStep) {
  auto const selectionStart = 2;
  auto const selectionLength = 3;
  givenSelection(selectionStart, selectionLength); // [2 3 4]

  auto const selectionEnd = selectionStart + selectionLength - 1;
  givenEnabled(selectionEnd, true);

  ASSERT_EQ(selector.successor(selectionStart), selectionEnd);
}

TEST_F(StepSelectorTest, successor_ifNoSelectedStepIsEnabled_isNegative) {
  auto const selectionStart = 1;
  auto const selectionLength = 4;
  givenSelection(selectionStart, selectionLength); // [1 2 3 4]

  givenEnabled(selectionStart + selectionLength, true); // Enabled but not in selection

  ASSERT_LT(selector.successor(selectionStart), 0);
}

TEST_F(StepSelectorTest, successor_ifNoStepInWrappedSelectionIsEnabled_isNegative) {
  auto const selectionStart = 5;
  auto const selectionLength = 7;
  givenSelection(selectionStart, selectionLength); // [5 6 7 0 1 2 3]

  auto const unselectedStep = 4; // The only enabled step is not in the selection
  givenEnabled(unselectedStep, true);

  ASSERT_LE(selector.successor(selectionStart), 0);
}

TEST_F(StepSelectorTest, successor_ifGivenStepIsBelowSelection_isNegative) {
  givenSelection(4, 3); // [4 5 6]

  auto const givenStep = 3; // Lower than any selected step
  givenEnabled(givenStep, true);

  ASSERT_LE(selector.successor(givenStep), 0);
}

TEST_F(StepSelectorTest, successor_ifGivenStepIsAboveSelection_isNegative) {
  givenSelection(4, 3); // [4 5 6]

  auto const givenStep = 7; // Higher than any selected step
  givenEnabled(givenStep, true);

  ASSERT_LE(selector.successor(givenStep), 0);
}

TEST_F(StepSelectorTest, successor_ifGivenStepIsBetweenEndpointsOfWrappedSelection_isNegative) {
  givenSelection(6, 4); // [6 7 0 1]

  auto const givenStep = 5; // Above selection end, below selection start
  givenEnabled(givenStep, true);

  ASSERT_LE(selector.successor(givenStep), 0);
}

TEST_F(StepSelectorTest, successor_ifFirstEnabledSuccessorIsBelowSelectionStart_returnsEnabledStep) {
  auto const selectionStart = 6;
  auto const selectionLength = 4;
  givenSelection(selectionStart, selectionLength); // [6 7 0 1]

  auto const enabledSelectedStep = 1; // Must wrap to find
  givenEnabled(enabledSelectedStep, true);

  ASSERT_EQ(selector.successor(selectionStart), enabledSelectedStep);
}

TEST_F(StepSelectorTest, first_ifSelectionStartIsEnabled_isSelectionStart) {
  auto constexpr selectionStart = 3;
  givenSelection(selectionStart, 4);
  givenEnabled(selectionStart, true);

  ASSERT_EQ(selector.first(), selectionStart);
}

TEST_F(StepSelectorTest, first_ifSelectionStartIsDisabled_isFirstEnabledSuccessor) {
  auto constexpr selectionStart = 3;
  givenSelection(selectionStart, 4);
  givenEnabled(selectionStart, false);
  auto constexpr firstEnabledSuccessor = 5;
  givenEnabled(firstEnabledSuccessor, true);

  ASSERT_EQ(selector.first(), firstEnabledSuccessor);
}

TEST_F(StepSelectorTest, first_ifNoSelectedStepIsEnabled_isNegative) {
  givenSelection(0, stepCount); // All disabled

  ASSERT_LT(selector.first(), 0);
}
