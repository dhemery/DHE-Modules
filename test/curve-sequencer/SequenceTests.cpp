#include "modules/curve-sequencer/Sequence.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

struct FakeStep {
  MOCK_METHOD(bool, isAvailable, ());
  MOCK_METHOD(void, process, (float));
};

struct FakeModule {
  MOCK_METHOD(bool, isRunning, ());
  MOCK_METHOD(bool, gate, ());
  MOCK_METHOD(int, startStep, ());
};

struct FakeLatch {
  MOCK_METHOD(void, step, (bool) );
  MOCK_METHOD(bool, high, (), (const));
  MOCK_METHOD(bool, rise, (), (const));
};

using ::testing::Return;
using ::testing::StrictMock;
using Sequence = dhe::curve_sequencer::Sequence<FakeModule, std::vector<FakeStep>, FakeLatch>;

float constexpr sampleTime = 1.F / 44000.F;

class SequenceTest : public ::testing::Test {
protected:
  FakeModule module;
  std::vector<FakeStep> steps{8};
  FakeLatch runLatch;
  FakeLatch gateLatch;
  Sequence sequence{module, steps, runLatch, gateLatch};

  void givenRunning(bool state, bool edge) {
    ON_CALL(module, isRunning()).WillByDefault(Return(state));
    ON_CALL(runLatch, step(state)).WillByDefault(Return());
    ON_CALL(runLatch, high()).WillByDefault(Return(state));
    ON_CALL(runLatch, rise()).WillByDefault(Return(edge && state));
  }

  void givenGate(bool state, bool edge) {
    ON_CALL(module, gate()).WillByDefault(Return(state));
    ON_CALL(gateLatch, step(state)).WillByDefault(Return());
    ON_CALL(gateLatch, rise()).WillByDefault(Return(edge && state));
  }

  void givenStartStep(int step) { ON_CALL(module, startStep()).WillByDefault(Return(step)); }

  void givenAvailable(int step, bool isAvailable) {
    ON_CALL(steps[step], isAvailable()).WillByDefault(Return(isAvailable));
  }
};

class SequenceNotRunning : public SequenceTest {
protected:
  void SetUp() override { givenRunning(false, false); }
};

TEST_F(SequenceNotRunning, processDoesNothing) { sequence.process(sampleTime); }

class SequenceRunningWithGateLow : public SequenceTest {
protected:
  void SetUp() override {
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

  EXPECT_CALL(steps[firstAvailableStep], process(sampleTime));

  sequence.process(sampleTime);
}
