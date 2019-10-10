#include "modules/components/Latch.h"
#include "modules/curve-sequencer/Sequence.h"
#include "modules/curve-sequencer/Step.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <mocks/MockSequenceControls.h>
#include <mocks/MockStep.h>
#include <vector>

using dhe::Latch;
using dhe::curve_sequencer::Sequence;
using dhe::curve_sequencer::Step;

float constexpr sampleTime = 1.F / 44000.F;
int constexpr stepCount = 8;

using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;

class NewSequence : public ::testing::Test {
protected:
  void SetUp() override {
    for (int i = 0; i < stepCount; i++) {
      auto step = new NiceMock<MockStep>{};
      ON_CALL(*step, isAvailable()).WillByDefault(Return(false));
      steps.emplace_back(step);
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
      ON_CALL(step(index), isAvailable()).WillByDefault(Return(true));
    }
  }

  auto step(int index) -> MockStep & { return *dynamic_cast<MockStep *>(steps[index].get()); }

  NiceMock<MockSequenceControls> controls;
  std::vector<std::unique_ptr<Step>> steps{};
  Sequence sequence{controls, steps};
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

  EXPECT_CALL(step(firstAvailableStep), process(A<Latch const &>(), sampleTime));

  givenGateInput(true); // Will trigger start of sequence

  sequence.process(sampleTime);
}

TEST_F(IdleSequence, ifNoAvailableStepAboveFirstSelected_continueSeekingFromStep0) {
  auto constexpr firstAvailableStep = 3;
  givenAvailableSteps({firstAvailableStep});

  auto constexpr selectionStart = 6; // Will have to wrap to reach first available step
  givenSelection(selectionStart, stepCount);

  EXPECT_CALL(step(firstAvailableStep), process(A<Latch const &>(), sampleTime));

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

  EXPECT_CALL(step(activeStep), process(A<Latch const &>(), sampleTime));

  sequence.process(sampleTime);
}

TEST_F(ActiveSequence, activatesSuccessorStep_ifActiveStepTerminates) {
  givenRunInput(true);

  ON_CALL(step(activeStep), process(A<Latch const &>(), sampleTime)).WillByDefault(Return(Step::State::Terminated));

  sequence.process(sampleTime);

  EXPECT_CALL(step(successorStep), process(A<Latch const &>(), sampleTime));

  sequence.process(sampleTime);
}