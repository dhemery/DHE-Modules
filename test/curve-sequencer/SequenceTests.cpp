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

using ::testing::NiceMock;
using ::testing::Return;

class SequenceTest : public ::testing::Test {
protected:
  NiceMock<MockLatch> runLatch;
  NiceMock<MockLatch> gateLatch;
  std::vector<MockStep *> mockSteps;
  std::vector<std::unique_ptr<Step>> steps{};
  Sequence sequence{runLatch, gateLatch, steps};

  void SetUp() override {
    for (int i = 0; i < 8; i++) {
      auto step = new NiceMock<MockStep>;
      mockSteps.push_back(step);
      steps.emplace_back(std::unique_ptr<Step>{step});
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

  void givenStartStep(int step) {}

  void givenAvailable(int step, bool isAvailable) {
    ON_CALL(*mockSteps[step], isAvailable()).WillByDefault(Return(isAvailable));
  }
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

TEST_F(SequenceRunningWithGateLow, gateRiseStartsFirstAvailableStep) {
  auto firstAvailableStep = 0;

  givenGate(true, true);
  givenStartStep(firstAvailableStep);
  givenAvailable(firstAvailableStep, true);

  EXPECT_CALL(*mockSteps[firstAvailableStep], process(sampleTime));

  sequence.process(sampleTime);
}
