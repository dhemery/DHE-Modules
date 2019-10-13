#include "modules/components/Latch.h"
#include "modules/curve-sequencer/Sequence.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

using dhe::Latch;
using dhe::curve_sequencer::Sequence;

static auto constexpr sampleTime = 1.F / 44000.F;
static auto constexpr stepCount = 8;

using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;

class MockSequenceControls {
public:
  MOCK_METHOD(float, curvature, (int), (const));
  MOCK_METHOD(float, duration, (int), (const));
  MOCK_METHOD(bool, gate, (), (const));
  MOCK_METHOD(int, generateMode, (int), (const));
  MOCK_METHOD(bool, isRunning, (), (const));
  MOCK_METHOD(bool, isEnabled, (int), (const));
  MOCK_METHOD(float, level, (int), (const));
  MOCK_METHOD(float, output, (), (const));
  MOCK_METHOD(int, selectionLength, (), (const));
  MOCK_METHOD(int, selectionStart, (), (const));
  MOCK_METHOD(int, sustainMode, (int), (const));
  MOCK_METHOD(int, taperSelection, (int), (const));

  MOCK_METHOD(void, setGenerating, (int, bool));
  MOCK_METHOD(void, setSustaining, (int, bool));
  MOCK_METHOD(void, setOutput, (float));
};

class MockStepExecutor {
public:
  MOCK_METHOD(bool, process, (int, Latch const&, float));
};

class NewSequence : public ::testing::Test {
protected:
  NiceMock<MockSequenceControls> controls;
  MockStepExecutor *stepExecutor = new NiceMock<MockStepExecutor>{};
  Sequence<MockSequenceControls, MockStepExecutor> sequence{controls, stepCount, stepExecutor};

  void SetUp() override {
    for (int i = 0; i < stepCount; i++) {
      givenSelection(0, stepCount);
    }
  };

  void givenRunInput(bool isRunning) { ON_CALL(controls, isRunning()).WillByDefault(Return(isRunning)); }

  void givenGateInput(bool gate) { ON_CALL(controls, gate()).WillByDefault(Return(gate)); }

  void givenSelection(int start, int length) {
    ON_CALL(controls, selectionStart()).WillByDefault(Return(start));
    ON_CALL(controls, selectionLength()).WillByDefault(Return(length));
  }

  void givenAvailableSteps(std::vector<int> const &indices) {
    for (auto const index : indices) {
    }
  }
};

class SequenceNotRunning : public NewSequence {
protected:
  void SetUp() override {
    NewSequence::SetUp();
    givenRunInput(false);
    sequence.process(0.F);
  }
};

TEST_F(SequenceNotRunning, processDoesNothing) { sequence.process(sampleTime); }

class IdleSequence : public NewSequence {
protected:
  void SetUp() override {
    NewSequence::SetUp();
    givenRunInput(true);
    givenGateInput(false);
    sequence.process(sampleTime);
  }
};

TEST_F(IdleSequence, gateRiseProcessesFirstAvailableStep) {
  givenSelection(0, stepCount);

  auto constexpr firstAvailableStep = 3;
  givenAvailableSteps({firstAvailableStep});

  EXPECT_CALL(*stepExecutor, process(firstAvailableStep, A<Latch const &>(), sampleTime));

  givenGateInput(true); // Will trigger start of sequence

  sequence.process(sampleTime);
}

TEST_F(IdleSequence, ifNoAvailableStepAboveFirstSelected_continueSeekingFromStep0) {
  auto constexpr firstAvailableStep = 3;
  givenAvailableSteps({firstAvailableStep});

  auto constexpr selectionStart = 6; // Will have to wrap to reach first available step
  givenSelection(selectionStart, stepCount);

  EXPECT_CALL(*stepExecutor, process(firstAvailableStep, A<Latch const &>(), sampleTime));

  givenGateInput(true); // Will trigger start of sequence

  sequence.process(sampleTime);
}

class ActiveSequence : public NewSequence {
protected:
  void SetUp() override {
    NewSequence::SetUp();

    givenRunInput(true);
    givenGateInput(true);
    givenAvailableSteps({activeStep, successorStep});
    sequence.process(sampleTime);
  }

  int const activeStep = 0;
  int const successorStep = 3;
};

TEST_F(ActiveSequence, processesActiveStep) {
  givenRunInput(true);

  EXPECT_CALL(*stepExecutor, process(activeStep, A<Latch const &>(), sampleTime));

  sequence.process(sampleTime);
}

TEST_F(ActiveSequence, activatesSuccessorStep_ifActiveStepTerminates) {
  givenRunInput(true);

  ON_CALL(*stepExecutor, process(activeStep, A<Latch const &>(), sampleTime)).WillByDefault(Return(false));

  sequence.process(sampleTime);

  EXPECT_CALL(*stepExecutor, process(successorStep, A<Latch const &>(), sampleTime));

  sequence.process(sampleTime);
}