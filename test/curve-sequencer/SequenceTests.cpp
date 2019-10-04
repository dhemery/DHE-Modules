#include "modules/components/Latch.h"
#include "modules/curve-sequencer/Sequence.h"
#include "modules/curve-sequencer/Step.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <mocks/MockLatch.h>
#include <mocks/MockStep.h>
#include <mocks/MockSequenceControls.h>

using dhe::Latch;
using dhe::curve_sequencer::Sequence;
using dhe::curve_sequencer::Step;

float constexpr sampleTime = 1.F / 44000.F;
int constexpr stepCount = 8;

using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;

class SequenceTest : public ::testing::Test {
protected:
  NiceMock<MockSequenceControls> controls;
  NiceMock<MockLatch> runLatch;
  NiceMock<MockLatch> gateLatch;
  std::vector<std::unique_ptr<Step>> steps{};
  Sequence sequence{controls, runLatch, gateLatch, steps};

  void SetUp() override {
    for (int i = 0; i < stepCount; i++) {
      auto step = new NiceMock<MockStep>;
      steps.emplace_back(step);
    }
  };
  void givenRunning(bool state, bool edge) {
    ON_CALL(runLatch, isHigh()).WillByDefault(Return(state));
    ON_CALL(runLatch, isEdge()).WillByDefault(Return(edge));
  }

  void givenGate(bool state, bool edge) {
    ON_CALL(gateLatch, isHigh()).WillByDefault(Return(state));
    ON_CALL(gateLatch, isEdge()).WillByDefault(Return(edge));
  }

  void givenSelection(int start, int length) {
    ON_CALL(controls, selectionStart()).WillByDefault(Return(start));
    ON_CALL(controls, selectionLength()).WillByDefault(Return(length));
  }

  void givenAvailable(int index, bool isAvailable) {
    ON_CALL(step(index), isAvailable()).WillByDefault(Return(isAvailable));
  }

  auto step(int index) -> MockStep & { return *dynamic_cast<MockStep *>(steps[index].get()); }
};

class SequenceNotRunning : public SequenceTest {
protected:
  void SetUp() override {
    SequenceTest::SetUp();
    givenRunning(false, false);
  }
};

TEST_F(SequenceNotRunning, processDoesNothing) { sequence.process(sampleTime); }

class SequenceRunningWithGateLow : public SequenceTest {
protected:
  void SetUp() override {
    SequenceTest::SetUp();
    givenRunning(true, false);
    givenGate(false, false);
    sequence.process(sampleTime);
  }
};

TEST_F(SequenceRunningWithGateLow, gateRiseProcessesFirstAvailableStep) {
  givenSelection(0, stepCount);

  auto firstAvailableStep = 3;

  givenGate(true, true);

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

TEST_F(SequenceRunningWithGateLow, ifNoAvailableStepAboveFirstSelected_continueSeekingFromStep0) {
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

  givenGate(true, true);

  sequence.process(sampleTime);
}
