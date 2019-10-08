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
  NiceMock<MockSequenceControls> controls;
  std::vector<std::unique_ptr<Step>> steps{};
  Sequence sequence{controls, steps};

  void SetUp() override {
    for (int i = 0; i < stepCount; i++) {
      auto step = new NiceMock<MockStep>;
      steps.emplace_back(step);
    }
  };

  void givenRunInput(bool isRunning) { ON_CALL(controls, isRunning()).WillByDefault(Return(isRunning)); }

  void givenGateInput(bool gate) { ON_CALL(controls, gate()).WillByDefault(Return(gate)); }

  void givenSelection(int start, int length) {
    ON_CALL(controls, selectionStart()).WillByDefault(Return(start));
    ON_CALL(controls, selectionLength()).WillByDefault(Return(length));
  }

  void givenAvailable(int index, bool isAvailable) {
    ON_CALL(step(index), isAvailable()).WillByDefault(Return(isAvailable));
  }

  auto step(int index) -> MockStep & { return *dynamic_cast<MockStep *>(steps[index].get()); }
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

  auto firstAvailableStep = 3;

  givenGateInput(true);

  for (int i = 0; i < firstAvailableStep; i++) {
    ON_CALL(step(i), isAvailable()).WillByDefault(Return(false));
    EXPECT_CALL(step(i), process(A<Latch const &>(), A<float>())).Times(0);
  }

  for (int i = firstAvailableStep + 1; i < stepCount; i++) {
    EXPECT_CALL(step(i), isAvailable()).Times(0);
    EXPECT_CALL(step(i), process(A<Latch const &>(), A<float>())).Times(0);
  }

  ON_CALL(step(firstAvailableStep), isAvailable()).WillByDefault(Return(true));
  EXPECT_CALL(step(firstAvailableStep), process(A<Latch const &>(), sampleTime));

  sequence.process(sampleTime);
}

TEST_F(IdleSequence, ifNoAvailableStepAboveFirstSelected_continueSeekingFromStep0) {
  auto constexpr firstSelected = 6; // Will have to wrap to reach first available
  givenSelection(firstSelected, stepCount);

  // Will check 6 and 7
  for (int i = firstSelected; i < stepCount; i++) {
    ON_CALL(step(i), isAvailable()).WillByDefault(Return(false));
    EXPECT_CALL(step(i), process(A<Latch const &>(), A<float>())).Times(0);
  }

  auto constexpr firstAvailable = 3;

  // Will check 0, 1, 2
  for (int i = 0; i < firstAvailable; i++) {
    ON_CALL(step(i), isAvailable()).WillByDefault(Return(false));
    EXPECT_CALL(step(i), process(A<Latch const &>(), A<float>())).Times(0);
  }

  ON_CALL(step(firstAvailable), isAvailable()).WillByDefault(Return(true));
  EXPECT_CALL(step(firstAvailable), process(A<Latch const &>(), sampleTime));

  // Will not check 4, 5
  for (int i = firstAvailable + 1; i < firstSelected; i++) {
    EXPECT_CALL(step(i), isAvailable()).Times(0);
    EXPECT_CALL(step(i), process(A<Latch const &>(), A<float>())).Times(0);
  }

  givenGateInput(true);

  sequence.process(sampleTime);
}
