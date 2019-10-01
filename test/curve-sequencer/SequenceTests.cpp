#include "modules/curve-sequencer/Sequence.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

using dhe::Latch;
using dhe::curve_sequencer::Sequence;
using dhe::curve_sequencer::Step;

struct MockLatch : public Latch {
  MOCK_METHOD(void, step, (), (override));
  MOCK_METHOD(void, set, (bool, bool), (override));
  MOCK_METHOD(bool, isHigh, (), (const, override));
  MOCK_METHOD(bool, isEdge, (), (const, override));
};

struct MockStep : public Step {
  MOCK_METHOD(bool, isAvailable, (), (const, override));
  MOCK_METHOD(void, process, (float), (override));
};

float constexpr sampleTime = 1.F / 44000.F;
int constexpr stepCount = 8;

using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;

class SequenceTest : public ::testing::Test {
  int start;
  int length = stepCount;

protected:
  std::function<int()> selectionStart{[this]() -> int { return start; }};
  std::function<int()> selectionLength{[this]() -> int { return length; }};
  NiceMock<MockLatch> runLatch;
  NiceMock<MockLatch> gateLatch;
  std::vector<std::unique_ptr<Step>> steps{};
  Sequence sequence{runLatch, gateLatch, selectionStart, selectionLength, steps};

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
    this->start = start;
    this->length = length;
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
    EXPECT_CALL(step(i), process(A<float>())).Times(0);
  }

  for (int i = firstAvailableStep + 1; i < stepCount; i++) {
    EXPECT_CALL(step(i), isAvailable()).Times(0);
    EXPECT_CALL(step(i), process(A<float>())).Times(0);
  }

  ON_CALL(step(firstAvailableStep), isAvailable()).WillByDefault(Return(true));
  EXPECT_CALL(step(firstAvailableStep), process(sampleTime));

  sequence.process(sampleTime);
}

TEST_F(SequenceRunningWithGateLow, ifNoAvailableStepAboveFirstSelected_continueSeekingFromStep0) {
  auto constexpr firstSelected = 6; // Will have to wrap to reach first available
  givenSelection(firstSelected, stepCount);

  // Will check 6 and 7
  for (int i = firstSelected; i < stepCount; i++) {
    ON_CALL(step(i), isAvailable()).WillByDefault(Return(false));
    EXPECT_CALL(step(i), process(A<float>())).Times(0);
  }

  auto constexpr firstAvailable = 3;

  // Will check 0, 1, 2
  for (int i = 0; i < firstAvailable; i++) {
    ON_CALL(step(i), isAvailable()).WillByDefault(Return(false));
    EXPECT_CALL(step(i), process(A<float>())).Times(0);
  }

  ON_CALL(step(firstAvailable), isAvailable()).WillByDefault(Return(true));
  EXPECT_CALL(step(firstAvailable), process(sampleTime));

  // Will not check 4, 5
  for (int i = firstAvailable + 1; i < firstSelected; i++) {
    EXPECT_CALL(step(i), isAvailable()).Times(0);
    EXPECT_CALL(step(i), process(A<float>())).Times(0);
  }

  givenGate(true, true);

  sequence.process(sampleTime);
}
