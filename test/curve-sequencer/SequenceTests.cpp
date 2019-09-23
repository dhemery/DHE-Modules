#include "modules/curve-sequencer/Sequence.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>

struct FakeStep {
  MOCK_METHOD(bool, isAvailable, ());
  MOCK_METHOD(void, start, ());
};

struct FakeModule {
  MOCK_METHOD(bool, isRunning, ());
  MOCK_METHOD(bool, gate, ());
  MOCK_METHOD(int, startStep, ());
  MOCK_METHOD(FakeStep &, step, (int) );
};

using ::testing::Return;
using ::testing::StrictMock;
using Sequence = dhe::curve_sequencer::Sequence<FakeModule, std::vector<FakeStep>>;

float constexpr sampleTime = 1.F / 44000.F;

struct WhileNotRunning : public ::testing::Test {
  StrictMock<FakeModule> module;
  std::vector<FakeStep> steps{};
  Sequence sequence = Sequence{module, steps};

  void SetUp() override { EXPECT_CALL(module, isRunning()).WillOnce(Return(false)); }
};

// TODO: How to positively set it to idle?
struct WhileIdle : public ::testing::Test {
  FakeModule module;
  std::vector<FakeStep> steps{4};
  Sequence sequence = Sequence{module, steps};
  void SetUp() override { EXPECT_CALL(module, isRunning()).WillOnce(Return(true)); }
};

TEST_F(WhileNotRunning, doesNothing) { sequence.process(sampleTime); }

TEST_F(WhileIdle, gateRiseStartsFirstAvailableStep) {
  auto firstAvailableStep = 0;
  ON_CALL(module, gate()).WillByDefault(Return(true));
  ON_CALL(module, startStep()).WillByDefault(Return(firstAvailableStep));
  ON_CALL(steps[firstAvailableStep], isAvailable()).WillByDefault(Return(true));

  EXPECT_CALL(steps[firstAvailableStep], start());

  sequence.process(sampleTime);
}
