#include "modules/components/Latch.h"
#include "modules/curve-sequencer/Sequence.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

using dhe::Latch;
using dhe::curve_sequencer::Sequence;

static auto constexpr defaultSampleTime = 1.F / 44100.F;
static auto constexpr stepCount = 8;

using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;

class MockSequenceControls {
public:
  MOCK_METHOD(bool, gate, (), (const));
  MOCK_METHOD(bool, isRunning, (), (const));
  MOCK_METHOD(int, selectionLength, (), (const));
  MOCK_METHOD(int, selectionStart, (), (const));
};

class MockStepExecutor {
public:
  MOCK_METHOD(bool, process, (int, Latch const &, float) );
};

class NewSequence : public ::testing::Test {
protected:
  NiceMock<MockSequenceControls> controls;
  MockStepExecutor *stepExecutor = new NiceMock<MockStepExecutor>{};
  Sequence<MockSequenceControls, MockStepExecutor> sequence{controls, stepCount, stepExecutor};

  void givenRunInput(bool isRunning) { ON_CALL(controls, isRunning()).WillByDefault(Return(isRunning)); }

  void givenGateInput(bool gate) { ON_CALL(controls, gate()).WillByDefault(Return(gate)); }

  void givenSelection(int start, int length) {
    ON_CALL(controls, selectionStart()).WillByDefault(Return(start));
    ON_CALL(controls, selectionLength()).WillByDefault(Return(length));
  }

  void givenActiveSteps(std::vector<int> const &indices) {
    for (auto const index : indices) {
      auto const gateLatch = A<Latch const &>();
      auto const sampleTime = A<float>();
      ON_CALL(*stepExecutor, process(index, gateLatch, sampleTime)).WillByDefault(Return(true));
    }
  }

  void SetUp() override {
    for (int i = 0; i < stepCount; i++) {
      givenSelection(0, stepCount);
    }
  };
};

class SequenceNotRunning : public NewSequence {
protected:
  void SetUp() override {
    NewSequence::SetUp();
    givenRunInput(false);
    sequence.process(0.F);
  }
};

TEST_F(SequenceNotRunning, processDoesNothing) { sequence.process(0.1F); }

class SequenceIdle : public NewSequence {
protected:
  void SetUp() override {
    NewSequence::SetUp();
    givenRunInput(true);
    givenGateInput(false);
    sequence.process(0.F);
  }
};

TEST_F(SequenceIdle, gateRise_processesEachStepFromFirstSelectedStepThroughFirstActiveStep) {
  givenSelection(0, stepCount);

  givenGateInput(true); // Will trigger start of sequence

  EXPECT_CALL(*stepExecutor, process(0, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  EXPECT_CALL(*stepExecutor, process(1, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  EXPECT_CALL(*stepExecutor, process(2, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  EXPECT_CALL(*stepExecutor, process(3, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(true));

  sequence.process(defaultSampleTime);
}

TEST_F(SequenceIdle, continueProcessingFromStep0_ifNoActiveStepAboveFirstSelectedStep) {
  auto constexpr firstActiveStep = 3;
  auto constexpr selectionStart = 6; // Will have to wrap to reach first active step
  givenSelection(selectionStart, stepCount);

  // Will try selectionStart and higher
  for (int step = selectionStart; step < stepCount; step++) {
    EXPECT_CALL(*stepExecutor, process(step, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  }

  // Then will try step 0 through the step before the first active step
  for (int step = 0; step < firstActiveStep; step++) {
    EXPECT_CALL(*stepExecutor, process(step, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  }

  // Finally, will try the first active step, then stop
  EXPECT_CALL(*stepExecutor, process(firstActiveStep, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(true));

  givenGateInput(true); // Will trigger start of sequence

  sequence.process(defaultSampleTime);
}

TEST_F(SequenceIdle, processesEachSelectedStepExactlyOnce_ifNoActiveStep) {
  auto constexpr selectionStart = 0;
  auto constexpr selectionLength = stepCount;
  givenSelection(selectionStart, selectionLength);

  for (int step = selectionStart; step < selectionStart + selectionLength; step++) {
    EXPECT_CALL(*stepExecutor, process(step, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  }

  givenGateInput(true); // Will trigger start of sequence

  sequence.process(defaultSampleTime);
}

TEST_F(SequenceIdle, processesOnlySelectedSteps) {
  auto constexpr selectionStart = 3;
  auto constexpr selectionLength = 2;
  givenSelection(selectionStart, selectionLength);

  for (int step = 0; step < selectionStart; step++) {
    EXPECT_CALL(*stepExecutor, process(step, A<Latch const &>(), defaultSampleTime)).Times(0);
  }

  for (int step = selectionStart; step < selectionStart + selectionLength; step++) {
    EXPECT_CALL(*stepExecutor, process(step, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  }

  for (int step = selectionStart + selectionLength; step < stepCount; step++) {
    EXPECT_CALL(*stepExecutor, process(step, A<Latch const &>(), defaultSampleTime)).Times(0);
  }

  givenGateInput(true); // Will trigger start of sequence

  sequence.process(defaultSampleTime);
}

class SequenceActive : public NewSequence {
protected:
  static auto constexpr previouslyActiveStep = 3;

  void SetUp() override {
    NewSequence::SetUp();

    givenSelection(0, stepCount);
    givenRunInput(true);
    givenGateInput(true);
    givenActiveSteps({previouslyActiveStep});

    sequence.process(0.F);
  }
};

TEST_F(SequenceActive, processesOnlyPreviouslyActiveStep_ifPreviouslyActiveStepIsStillActive) {
  givenRunInput(true);

  for (int step = 0; step < previouslyActiveStep; step++) {
    EXPECT_CALL(*stepExecutor, process(step, A<Latch const &>(), defaultSampleTime)).Times(0);
  }

  EXPECT_CALL(*stepExecutor, process(previouslyActiveStep, A<Latch const &>(), defaultSampleTime))
      .WillOnce(Return(true));

  for (int step = previouslyActiveStep + 1; step < stepCount; step++) {
    EXPECT_CALL(*stepExecutor, process(step, A<Latch const &>(), defaultSampleTime)).Times(0);
  }

  sequence.process(defaultSampleTime);
}

TEST_F(SequenceActive, processesSucessorSteps_ifPreviouslyActiveStepIsNoLongerActive) {
  givenRunInput(true);

  auto constexpr firstActiveSuccessorStep = previouslyActiveStep + 3;

  for (int step = previouslyActiveStep; step < firstActiveSuccessorStep; step++) {
    EXPECT_CALL(*stepExecutor, process(step, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  }

  EXPECT_CALL(*stepExecutor, process(firstActiveSuccessorStep, A<Latch const &>(), defaultSampleTime))
      .WillOnce(Return(true));

  sequence.process(defaultSampleTime);
}
