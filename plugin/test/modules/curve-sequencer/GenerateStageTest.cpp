#include "components/Latch.h"
#include "curve-sequencer/CurveSequencerControls.h"
#include "curve-sequencer/Stages.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mocks/MockLight.h>
#include <mocks/MockParam.h>
#include <mocks/MockPort.h>
#include <vector>

static auto constexpr stepCount = 4;
static auto constexpr defaultStep = 3;
static auto constexpr defaultSampleTime = 0.F;

using dhe::Latch;
using dhe::curve_sequencer::GenerateStage;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;

using ::testing::A;
using ::testing::An;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

using InputType = NiceMock<MockPort>;
using OutputType = NiceMock<MockPort>;
using ParamType = NiceMock<MockParam>;
using LightType = NiceMock<MockLight>;

static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr stableHighGate = Latch{true, false};
static auto constexpr stableLowGate = Latch{false, false};

class GenerateStageTest : public Test {
protected:
  std::vector<InputType> inputs{Controls::InputCount};
  std::vector<OutputType> outputs{Controls::OutputCount};
  std::vector<ParamType> params{Controls::ParameterCount};
  std::vector<LightType> lights{Controls::LightCount};
  GenerateStage<stepCount, InputType, OutputType, ParamType, LightType> generateStage{inputs, outputs, params, lights};

  void setMode(int mode) {
    ON_CALL(params[Controls::GenerateModeSwitches + defaultStep], getValue())
        .WillByDefault(Return(static_cast<float>(mode)));
  }

  auto generatingLight(int step) -> LightType & { return lights[Controls::GeneratingLights + step]; }
};

class GenerateStageRiseMode : public GenerateStageTest {
protected:
  void SetUp() override {
    GenerateStageTest::SetUp();
    setMode(dhe::curve_sequencer::riseMode);
  }
};

TEST_F(GenerateStageRiseMode, isActive_ifGateDoesNotRise) {
  EXPECT_CALL(generatingLight(defaultStep), setBrightness(10.F));

  auto const isActive = generateStage.execute(defaultStep, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, true);
}

TEST_F(GenerateStageRiseMode, isInactive_ifGateRises) {
  EXPECT_CALL(generatingLight(defaultStep), setBrightness(0.F));

  auto const isActive = generateStage.execute(defaultStep, risenGate, defaultSampleTime);

  EXPECT_EQ(isActive, false);
}

class GenerateStageFallMode : public GenerateStageTest {
protected:
  void SetUp() override {
    GenerateStageTest::SetUp();
    setMode(dhe::curve_sequencer::fallMode);
  }
};

TEST_F(GenerateStageFallMode, isActive_ifGateDoesNotFall) {
  EXPECT_CALL(generatingLight(defaultStep), setBrightness(10.F));

  auto const isActive = generateStage.execute(defaultStep, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, true);
}

TEST_F(GenerateStageFallMode, isInactive_ifGateFalls) {
  EXPECT_CALL(generatingLight(defaultStep), setBrightness(0.F));

  auto const isActive = generateStage.execute(defaultStep, fallenGate, defaultSampleTime);

  EXPECT_EQ(isActive, false);
}

class GenerateStageEdgeMode : public GenerateStageTest {
protected:
  void SetUp() override {
    GenerateStageTest::SetUp();
    setMode(dhe::curve_sequencer::edgeMode);
  }
};

TEST_F(GenerateStageEdgeMode, isActive_ifGateDoesNotChange) {
  EXPECT_CALL(generatingLight(defaultStep), setBrightness(10.F));

  auto const isActive = generateStage.execute(defaultStep, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, true);
}

TEST_F(GenerateStageEdgeMode, isInactive_ifGateChanges) {
  EXPECT_CALL(generatingLight(defaultStep), setBrightness(0.F));

  auto const isActive = generateStage.execute(defaultStep, risenGate, defaultSampleTime);

  EXPECT_EQ(isActive, false);
}

class GenerateStageHighMode : public GenerateStageTest {
protected:
  void SetUp() override {
    GenerateStageTest::SetUp();
    setMode(dhe::curve_sequencer::highMode);
  }
};

TEST_F(GenerateStageHighMode, isActive_ifGateIsLow) {
  EXPECT_CALL(generatingLight(defaultStep), setBrightness(10.F));

  auto const isActive = generateStage.execute(defaultStep, stableLowGate, defaultSampleTime);

  EXPECT_EQ(isActive, true);
}

TEST_F(GenerateStageHighMode, isInactive_ifGateIsHigh) {
  EXPECT_CALL(generatingLight(defaultStep), setBrightness(0.F));

  auto const isActive = generateStage.execute(defaultStep, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, false);
}

class GenerateStageLowMode : public GenerateStageTest {
protected:
  void SetUp() override {
    GenerateStageTest::SetUp();
    setMode(dhe::curve_sequencer::lowMode);
  }
};

TEST_F(GenerateStageLowMode, isActive_ifGateIsHigh) {
  EXPECT_CALL(generatingLight(defaultStep), setBrightness(10.F));

  auto const isActive = generateStage.execute(defaultStep, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, true);
}

TEST_F(GenerateStageLowMode, isInactive_ifGateIsLow) {
  EXPECT_CALL(generatingLight(defaultStep), setBrightness(0.F));

  auto const isActive = generateStage.execute(defaultStep, stableLowGate, defaultSampleTime);

  EXPECT_EQ(isActive, false);
}

class GenerateStageSkipMode : public GenerateStageTest {
protected:
  void SetUp() override {
    GenerateStageTest::SetUp();
    setMode(dhe::curve_sequencer::skipMode);
  }
};

TEST_F(GenerateStageSkipMode, isInactive) {
  EXPECT_CALL(generatingLight(defaultStep), setBrightness(0.F));

  auto const isActive = generateStage.execute(defaultStep, Latch{}, defaultSampleTime);

  EXPECT_EQ(isActive, false);
}

class GenerateStageDurationMode : public GenerateStageTest {
protected:
  void SetUp() override {
    GenerateStageTest::SetUp();
    setMode(dhe::curve_sequencer::durationMode);
  }
};

TEST_F(GenerateStageDurationMode, isActive) {
  EXPECT_CALL(generatingLight(defaultStep), setBrightness(10.F));

  auto const isActive = generateStage.execute(defaultStep, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, true);
}
