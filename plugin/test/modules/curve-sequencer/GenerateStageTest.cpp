#include "components/Latch.h"
#include "curve-sequencer/CurveSequencerControls.h"
#include "curve-sequencer/Stages.h"

#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <gtest/gtest.h>
#include <vector>

static auto constexpr stepCount = 4;
static auto constexpr defaultSampleTime = 0.F;

using dhe::Latch;
using dhe::curve_sequencer::GenerateStage;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;

using ::testing::Test;

using InputType = rack::engine::Input;
using OutputType = rack::engine::Input;
using ParamType = rack::engine::Param;
using LightType = rack::engine::Light;

static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr stableHighGate = Latch{true, false};
static auto constexpr stableLowGate = Latch{false, false};

class GenerateStageTest : public Test {
protected:
  GenerateStage<stepCount, InputType, OutputType, ParamType, LightType> generateStage{inputs, outputs, params, lights};

  void givenGenerateMode(int step, int mode) {
    params[Controls::GenerateModeSwitches + step].setValue(static_cast<float>(mode));
  }

  void givenGeneratingLight(int step, float brightness) {
    lights[Controls::GeneratingLights + step].setBrightness(brightness);
  }

  auto generatingLight(int step) -> float { return lights[Controls::GeneratingLights + step].getBrightness(); }

private:
  std::vector<InputType> inputs{Controls::InputCount};
  std::vector<OutputType> outputs{Controls::OutputCount};
  std::vector<ParamType> params{Controls::ParameterCount};
  std::vector<LightType> lights{Controls::LightCount};
};

TEST_F(GenerateStageTest, stepIsActive_ifGateDoesNotRise_whenStepIsInRiseMode) {
  auto const step = 0;

  givenGenerateMode(step, dhe::curve_sequencer::riseMode);

  auto const isActive = generateStage.execute(step, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, true);
  EXPECT_EQ(generatingLight(step), 10.F);
}

TEST_F(GenerateStageTest, stepIsInactive_ifGateRises_whenStepIsInRiseMode) {
  auto const step = 1;
  givenGenerateMode(step, dhe::curve_sequencer::riseMode);
  givenGeneratingLight(step, 10.F);

  auto const isActive = generateStage.execute(step, risenGate, defaultSampleTime);

  EXPECT_EQ(isActive, false);
  EXPECT_EQ(generatingLight(step), 0.F);
}

class GenerateStageFallMode : public GenerateStageTest {
protected:
  void SetUp() override { GenerateStageTest::SetUp(); }
};

TEST_F(GenerateStageFallMode, stepIsActive_ifGateDoesNotFall_whenStepIsInFallMode) {
  auto const step = 2;
  givenGenerateMode(step, dhe::curve_sequencer::fallMode);

  auto const isActive = generateStage.execute(step, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, true);
  EXPECT_EQ(generatingLight(step), 10.F);
}

TEST_F(GenerateStageFallMode, stepBecomesInactive_ifStepIsInFallMode_ifGateFalls) {
  auto const step = 2;
  givenGenerateMode(step, dhe::curve_sequencer::edgeMode);

  auto const isActive = generateStage.execute(step, fallenGate, defaultSampleTime);

  EXPECT_EQ(isActive, false);
  EXPECT_EQ(generatingLight(step), 0.F);
}

class GenerateStageEdgeMode : public GenerateStageTest {
protected:
  void SetUp() override { GenerateStageTest::SetUp(); }
};

TEST_F(GenerateStageEdgeMode, isActive_ifGateDoesNotChange) {
  auto const step = 3;
  givenGenerateMode(step, dhe::curve_sequencer::edgeMode);

  auto const isActive = generateStage.execute(step, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, true);
  EXPECT_EQ(generatingLight(step), 10.F);
}

TEST_F(GenerateStageEdgeMode, isInactive_ifGateChanges) {
  auto const step = 0;
  givenGenerateMode(step, dhe::curve_sequencer::edgeMode);

  auto const isActive = generateStage.execute(step, risenGate, defaultSampleTime);

  EXPECT_EQ(isActive, false);
  EXPECT_EQ(generatingLight(step), 0.F);
}

class GenerateStageHighMode : public GenerateStageTest {
protected:
  void SetUp() override { GenerateStageTest::SetUp(); }
};

TEST_F(GenerateStageHighMode, isActive_ifGateIsLow) {
  auto const step = 1;
  givenGenerateMode(step, dhe::curve_sequencer::highMode);

  auto const isActive = generateStage.execute(step, stableLowGate, defaultSampleTime);

  EXPECT_EQ(isActive, true);
  EXPECT_EQ(generatingLight(step), 10.F);
}

TEST_F(GenerateStageHighMode, isInactive_ifGateIsHigh) {
  auto const step = 2;
  givenGenerateMode(step, dhe::curve_sequencer::highMode);
  auto const isActive = generateStage.execute(step, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, false);
  EXPECT_EQ(generatingLight(step), 0.F);
}

class GenerateStageLowMode : public GenerateStageTest {
protected:
  void SetUp() override { GenerateStageTest::SetUp(); }
};

TEST_F(GenerateStageLowMode, isActive_ifGateIsHigh) {
  auto const step = 3;
  givenGenerateMode(step, dhe::curve_sequencer::lowMode);

  auto const isActive = generateStage.execute(step, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, true);
  EXPECT_EQ(generatingLight(step), 10.F);
}

TEST_F(GenerateStageLowMode, isInactive_ifGateIsLow) {
  auto const step = 0;
  givenGenerateMode(step, dhe::curve_sequencer::lowMode);

  auto const isActive = generateStage.execute(step, stableLowGate, defaultSampleTime);

  EXPECT_EQ(isActive, false);
  EXPECT_EQ(generatingLight(step), 0.F);
}

class GenerateStageSkipMode : public GenerateStageTest {
protected:
  void SetUp() override { GenerateStageTest::SetUp(); }
};

TEST_F(GenerateStageSkipMode, isInactive) {
  auto const step = 1;
  givenGenerateMode(step, dhe::curve_sequencer::skipMode);

  auto const isActive = generateStage.execute(step, Latch{}, defaultSampleTime);

  EXPECT_EQ(isActive, false);
  EXPECT_EQ(generatingLight(step), 0.F);
}

class GenerateStageDurationMode : public GenerateStageTest {
protected:
  void SetUp() override { GenerateStageTest::SetUp(); }
};

TEST_F(GenerateStageDurationMode, isActive) {
  auto const step = 2;
  givenGenerateMode(step, dhe::curve_sequencer::durationMode);

  auto const isActive = generateStage.execute(step, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, true);
  EXPECT_EQ(generatingLight(step), 10.F);
}
