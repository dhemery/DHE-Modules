#include "curve-sequencer/StepSelector.h"

#include "components/Latch.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::StepSelector;

using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class MockControls {
public:
  MOCK_METHOD(int, selectionStart, (), (const));
  MOCK_METHOD(int, selectionLength, (), (const));
};

class MockStage {
public:
  MOCK_METHOD(bool, isAvailable, (int, Latch const &), (const));
};

struct StepSelectorTest : public Test {
  static auto constexpr stepCount{8};
  NiceMock<MockControls> controls{};
  NiceMock<MockStage> generateStage{};
  StepSelector<MockControls, MockStage> selector{controls, generateStage, stepCount};

  void givenAvailable(int step) {
    ON_CALL(generateStage, isAvailable(step, A<Latch const &>())).WillByDefault(Return(true));
  }

  void givenSelection(int start, int length) {
    ON_CALL(controls, selectionStart()).WillByDefault(Return(start));
    ON_CALL(controls, selectionLength()).WillByDefault(Return(length));
  }
};

TEST_F(StepSelectorTest, findsSuccessorAboveGivenStep) {
  givenSelection(0, stepCount); // [0 1 2 3 4 5 6 7]
  auto const givenStep = 2;
  auto const expectedSuccessor = givenStep + 2; // Later in selection
  givenAvailable(expectedSuccessor);

  auto const successor = selector.successor(givenStep, Latch{}, false);

  ASSERT_EQ(successor, expectedSuccessor);
}

TEST_F(StepSelectorTest, findsSuccessorBelowGivenStep_ifSelectionWraps) {
  auto const selectionStart = 6;
  auto const selectionLength = 4;
  givenSelection(selectionStart, selectionLength); // 0 1] 2 3 4 5 [6 7
  auto const expectedSuccessor = 1;                // Must wrap to find
  givenAvailable(expectedSuccessor);

  auto const successor = selector.successor(selectionStart, Latch{}, false);

  ASSERT_EQ(successor, expectedSuccessor);
}

TEST_F(StepSelectorTest, selectionEndCanBeSuccessor) {
  auto const selectionStart = 2;
  auto const selectionLength = 3;
  givenSelection(selectionStart, selectionLength); // 0 1 [2 3 4] 5 6 7
  auto const selectionEnd = selectionStart + selectionLength - 1;
  givenAvailable(selectionEnd);

  auto const successor = selector.successor(selectionStart, Latch{}, false);

  ASSERT_EQ(successor, selectionEnd);
}

TEST_F(StepSelectorTest, noSuccessor_ifNoSelectedStepIsEnabled) {
  auto const selectionStart = 1;
  auto const selectionLength = 4;
  givenSelection(selectionStart, selectionLength); // 0 [1 2 3 4] 5 6 7
  auto const onlyEnabledStep = 6;
  givenAvailable(onlyEnabledStep); // Enabled but not in selection

  auto const successor = selector.successor(selectionStart, Latch{}, true);

  ASSERT_LT(successor, 0);
}

TEST_F(StepSelectorTest, noSuccessor_ifSelectionWraps_andNoSelectedStepIsEnabled) {
  auto const selectionStart = 5;
  auto const selectionLength = 7;
  givenSelection(selectionStart, selectionLength); // 0 1 2 3] 4 [5 6 7
  auto const onlyEnabledStep = 4;                  // The only enabled step is not in the selection
  givenAvailable(onlyEnabledStep);

  auto const successor = selector.successor(selectionStart, Latch{}, true);

  ASSERT_LE(successor, 0);
}

TEST_F(StepSelectorTest, noSuccessor_ifGivenStepIsBelowSelection) {
  givenSelection(4, 3);     // 0 1 2 3 [4 5 6] 7
  auto const givenStep = 3; // Lower than any selected step
  givenAvailable(givenStep);

  auto const successor = selector.successor(givenStep, Latch{}, true);

  ASSERT_LE(successor, 0);
}

TEST_F(StepSelectorTest, noSuccessor_ifGivenStepIsAboveSelection) {
  givenSelection(4, 3);     // 0 1 2 3 [4 5 6] 7
  auto const givenStep = 7; // Higher than any selected step
  givenAvailable(givenStep);

  auto const successor = selector.successor(givenStep, Latch{}, true);

  ASSERT_LE(successor, 0);
}

TEST_F(StepSelectorTest, noSuccessor_ifGivenStepNotIncludedInWrappedSelection) {
  givenSelection(6, 4);     // 0 1] 2 3 4 5 [6 7
  auto const givenStep = 5; // Above selection end, below selection start
  givenAvailable(givenStep);

  auto const successor = selector.successor(givenStep, Latch{}, true);

  ASSERT_LE(successor, 0);
}

TEST_F(StepSelectorTest, noSuccessor_ifGivenStepIsSelectionEnd_andSequencerIsNotLooping) {
  auto constexpr step = 3;
  givenSelection(1, step); // 0 [1 2 3] 4 5 6 7

  auto const successor = selector.successor(step, Latch{}, false);

  ASSERT_LT(successor, 0);
}

TEST_F(StepSelectorTest, first_returnsSelectionStart_ifSelectionStartIsEnabled) {
  auto constexpr selectionStart = 3;
  givenSelection(selectionStart, 4);
  givenAvailable(selectionStart);

  auto const first = selector.first(Latch{});

  ASSERT_EQ(first, selectionStart);
}

TEST_F(StepSelectorTest, first_returnsSuccessorOfSelectionStart_ifSelectionStartIsDisabled) {
  auto constexpr selectionStart = 3;
  givenSelection(selectionStart, 4); // Not enabled
  auto constexpr enabledSelectedStep = 5;
  givenAvailable(enabledSelectedStep);

  auto const first = selector.first(Latch{});
  auto const successorOfSelectionStart = selector.successor(selectionStart, Latch{}, false);

  ASSERT_EQ(first, successorOfSelectionStart);
}

TEST_F(StepSelectorTest, noFirst_ifNoSelectedStepIsEnabled) {
  givenSelection(0, stepCount); // All selected, but none enabled

  auto const first = selector.first(Latch{});

  ASSERT_LT(first, 0);
}

TEST_F(StepSelectorTest, findsSuccessorEarlierInSelection_ifIfSequencerIsLooping) {
  givenSelection(0, stepCount); // [0 1 2 3 4 5 6 7]
  auto constexpr step = 3;
  auto constexpr expectedSuccessor = step - 2; // Earlier in selection
  givenAvailable(expectedSuccessor);

  auto const successor = selector.successor(step, Latch{}, true);

  ASSERT_EQ(successor, expectedSuccessor);
}

TEST_F(StepSelectorTest, givenStepCanBeItsOwnSuccessor_ifSequencerIsLooping) {
  givenSelection(0, stepCount); // [0 1 2 3 4 5 6 7]
  auto constexpr step = 3;
  givenAvailable(step); // The only enabled step

  auto const successor = selector.successor(step, Latch{}, true);

  ASSERT_EQ(successor, step);
}