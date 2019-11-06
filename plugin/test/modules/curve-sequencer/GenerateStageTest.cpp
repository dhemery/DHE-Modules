#include "components/Latch.h"
#include "curve-sequencer/CurveSequencerControls.h"
#include "curve-sequencer/GenerateStage.h"
#include "curve-sequencer/SequenceMode.h"
#include "curve-sequencer/StageMode.h"

#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <gtest/gtest.h>

static auto constexpr stepCount{8};

using dhe::Latch;
using dhe::curve_sequencer::GenerateStage;
using dhe::curve_sequencer::SequenceMode;
using dhe::curve_sequencer::StageMode;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;

static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr stableHighGate = Latch{true, false};
static auto constexpr stableLowGate = Latch{false, false};

using ::testing::Test;

struct GenerateStageTest : public Test {
  std::vector<rack::engine::Input> inputs{Controls::InputCount};
  std::vector<rack::engine::Param> params{Controls::ParameterCount};
  std::vector<rack::engine::Light> lights{Controls::LightCount};

  GenerateStage<stepCount> generateStage{inputs, params, lights};

  void setInterruptMode(int step, StageMode mode) {
    params[Controls::GenerateModeSwitches + step].setValue(static_cast<float>(mode));
  }
};

TEST_F(GenerateStageTest, enter_lightsStepGeneratingLight) {
  auto const step = 4;

  lights[Controls::GeneratingLights + step].setBrightness(22.F);

  generateStage.enter(step);

  EXPECT_EQ(lights[Controls::GeneratingLights + step].getBrightness(), 10.F);
}

TEST_F(GenerateStageTest, exit_dimsStepGeneratingLight) {
  auto const step = 5;

  generateStage.enter(step);
  generateStage.exit();

  EXPECT_EQ(lights[Controls::GeneratingLights + step].getBrightness(), 0.F);
}

TEST_F(GenerateStageTest, highMode_returnsGenerating_ifGateIsHigh) {
  auto const step = 7;
  setInterruptMode(step, StageMode::High);

  generateStage.enter(step);

  auto next = generateStage.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);

  next = generateStage.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);
}

TEST_F(GenerateStageTest, highMode_returnsSustaining_ifGateIsLow) {
  auto const step = 6;
  setInterruptMode(step, StageMode::High);

  generateStage.enter(step);

  auto next = generateStage.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generateStage.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}

TEST_F(GenerateStageTest, lowMode_returnsGenerating_ifGateIsLow) {
  auto const step = 1;
  setInterruptMode(step, StageMode::Low);

  generateStage.enter(step);

  auto next = generateStage.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);

  next = generateStage.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);
}

TEST_F(GenerateStageTest, lowMode_returnsSustaining_ifGateIsHigh) {
  auto const step = 0;
  setInterruptMode(step, StageMode::Low);

  generateStage.enter(step);

  auto next = generateStage.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generateStage.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}

TEST_F(GenerateStageTest, calmMode_returnsGenerating_ifGateIsStable) {
  auto const step = 5;
  setInterruptMode(step, StageMode::Calm);

  generateStage.enter(step);

  auto next = generateStage.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);

  next = generateStage.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);
}

TEST_F(GenerateStageTest, calmMode_returnsSustaining_ifGateChanges) {
  auto const step = 4;
  setInterruptMode(step, StageMode::Calm);

  generateStage.enter(step);

  auto next = generateStage.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generateStage.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}

TEST_F(GenerateStageTest, skipMode_returnsSustaining_forEveryGateState) {
  auto const step = 0;
  setInterruptMode(step, StageMode::Skip);

  generateStage.enter(step);

  auto next = generateStage.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generateStage.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generateStage.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generateStage.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}
