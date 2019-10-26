#include "StepExecutor.h"

#include "Latch.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::StepExecutor;

using ::testing::A;
using ::testing::An;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class MockGenerateStage {
public:
  MOCK_METHOD(bool, execute, (int, Latch const &, float) );
};

class MockSustainStage {
public:
  MOCK_METHOD(bool, execute, (int, Latch const &) );
};

class MockSequenceControls {
public:
  MOCK_METHOD(bool, isEnabled, (int), (const));
};

static auto constexpr defaultSampleTime = 1.F / 44100.F;

class StepExecutorNew : public Test {
protected:
  NiceMock<MockSequenceControls> controls;
  MockGenerateStage *generateStage = new NiceMock<MockGenerateStage>;
  MockSustainStage *sustainStage = new NiceMock<MockSustainStage>;
  StepExecutor<MockSequenceControls, MockGenerateStage, MockSustainStage> executor{controls, generateStage,
                                                                                   sustainStage};
};

class StepExecutorDisabled : public StepExecutorNew {
protected:
  static auto constexpr defaultStep = 3;

  void SetUp() override {
    StepExecutorNew::SetUp();
    ON_CALL(controls, isEnabled(defaultStep)).WillByDefault(Return(false));
  }
};

TEST_F(StepExecutorDisabled, executesNeitherStage) {
  EXPECT_CALL(*generateStage, execute(An<int>(), A<Latch const &>(), A<float>())).Times(0);
  EXPECT_CALL(*sustainStage, execute(An<int>(), A<Latch const &>())).Times(0);

  auto const executed = executor.execute(defaultStep, Latch{}, defaultSampleTime);

  EXPECT_FALSE(executed);
}

class StepExecutorEnabled : public StepExecutorNew {
protected:
  static auto constexpr defaultStep = 3;

  void SetUp() override {
    StepExecutorNew::SetUp();
    ON_CALL(controls, isEnabled(defaultStep)).WillByDefault(Return(true));
  }
};

TEST_F(StepExecutorEnabled, executesOnlyGenerateStage_ifGenerateStageActed) {
  EXPECT_CALL(*generateStage, execute(An<int>(), A<Latch const &>(), A<float>())).WillOnce(Return(true));
  EXPECT_CALL(*sustainStage, execute(An<int>(), A<Latch const &>())).Times(0);

  auto const executed = executor.execute(defaultStep, Latch{}, defaultSampleTime);

  EXPECT_TRUE(executed);
}

TEST_F(StepExecutorEnabled, executesSustainStage_ifGenerateStageDidNotAct) {
  EXPECT_CALL(*generateStage, execute(An<int>(), A<Latch const &>(), A<float>())).WillOnce(Return(false));
  EXPECT_CALL(*sustainStage, execute(An<int>(), A<Latch const &>())).WillOnce(Return(true));

  auto const executed = executor.execute(defaultStep, Latch{}, defaultSampleTime);

  EXPECT_TRUE(executed);
}

TEST_F(StepExecutorEnabled, returnsFalse_ifNeitherStageActed) {
  EXPECT_CALL(*generateStage, execute(An<int>(), A<Latch const &>(), A<float>())).WillOnce(Return(false));
  EXPECT_CALL(*sustainStage, execute(An<int>(), A<Latch const &>())).WillOnce(Return(false));

  auto const executed = executor.execute(defaultStep, Latch{}, defaultSampleTime);

  EXPECT_FALSE(executed);
}
