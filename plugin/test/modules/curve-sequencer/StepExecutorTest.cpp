#include "curve-sequencer/StepExecutor.h"

#include "components/Latch.h"
#include "curve-sequencer/CurveSequencerControls.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mocks/MockLight.h>
#include <mocks/MockParam.h>
#include <mocks/MockPort.h>

static auto constexpr stepCount = 4;
static auto constexpr defaultStep = 3;
static auto constexpr defaultSampleTime = 1.F / 44100.F;

using ::testing::A;
using ::testing::An;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

using InputType = NiceMock<MockPort>;
using OutputType = NiceMock<MockPort>;
using ParamType = NiceMock<MockParam>;
using LightType = NiceMock<MockLight>;

using dhe::Latch;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;
using dhe::curve_sequencer::StepExecutor;

class MockGenerateStage {
public:
  MOCK_METHOD(bool, execute, (int, Latch const &, float) );
};

class MockSustainStage {
public:
  MOCK_METHOD(bool, execute, (int, Latch const &) );
};

class StepExecutorNew : public Test {
protected:
  std::vector<InputType> inputs{Controls::InputCount};
  std::vector<OutputType> outputs{Controls::OutputCount};
  std::vector<ParamType> params{Controls::ParameterCount};
  std::vector<LightType> lights{Controls::LightCount};

  NiceMock<MockGenerateStage> generateStage{};
  NiceMock<MockSustainStage> sustainStage{};
  StepExecutor<stepCount, InputType, OutputType, ParamType, LightType, MockGenerateStage, MockSustainStage>
      stepExecutor{inputs, outputs, params, lights, generateStage, sustainStage};

protected:
  void setEnabled(bool state) {
    ON_CALL(params[Controls::EnabledButtons + defaultStep], getValue()).WillByDefault(Return(state ? 10.F : 0.F));
  }
};

class StepExecutorDisabled : public StepExecutorNew {
protected:
  void SetUp() override {
    StepExecutorNew::SetUp();
    setEnabled(false);
  }
};

TEST_F(StepExecutorDisabled, executesNeitherStage) {
  EXPECT_CALL(generateStage, execute(An<int>(), A<Latch const &>(), A<float>())).Times(0);
  EXPECT_CALL(sustainStage, execute(An<int>(), A<Latch const &>())).Times(0);

  auto const executed = stepExecutor.execute(defaultStep, Latch{}, defaultSampleTime);

  EXPECT_FALSE(executed);
}

class StepExecutorEnabled : public StepExecutorNew {
protected:
  static auto constexpr defaultStep = 3;

  void SetUp() override {
    StepExecutorNew::SetUp();
    setEnabled(true);
  }
};

TEST_F(StepExecutorEnabled, executesOnlyGenerateStage_ifGenerateStageActed) {
  EXPECT_CALL(generateStage, execute(An<int>(), A<Latch const &>(), A<float>())).WillOnce(Return(true));
  EXPECT_CALL(sustainStage, execute(An<int>(), A<Latch const &>())).Times(0);

  auto const executed = stepExecutor.execute(defaultStep, Latch{}, defaultSampleTime);

  EXPECT_TRUE(executed);
}

TEST_F(StepExecutorEnabled, executesSustainStage_ifGenerateStageDidNotAct) {
  EXPECT_CALL(generateStage, execute(An<int>(), A<Latch const &>(), A<float>())).WillOnce(Return(false));
  EXPECT_CALL(sustainStage, execute(An<int>(), A<Latch const &>())).WillOnce(Return(true));

  auto const executed = stepExecutor.execute(defaultStep, Latch{}, defaultSampleTime);

  EXPECT_TRUE(executed);
}

TEST_F(StepExecutorEnabled, returnsFalse_ifNeitherStageActed) {
  EXPECT_CALL(generateStage, execute(An<int>(), A<Latch const &>(), A<float>())).WillOnce(Return(false));
  EXPECT_CALL(sustainStage, execute(An<int>(), A<Latch const &>())).WillOnce(Return(false));

  auto const executed = stepExecutor.execute(defaultStep, Latch{}, defaultSampleTime);

  EXPECT_FALSE(executed);
}
