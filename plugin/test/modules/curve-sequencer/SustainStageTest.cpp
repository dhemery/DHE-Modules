#include "curve-sequencer/SustainStage.h"

#include "components/Latch.h"
#include "curve-sequencer/CurveSequencerControls.h"
#include "curve-sequencer/SequenceMode.h"
#include "curve-sequencer/StageMode.h"

#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <gtest/gtest.h>

static auto constexpr stepCount{8};

using dhe::Latch;
using dhe::curve_sequencer::SequenceMode;
using dhe::curve_sequencer::StageMode;
using dhe::curve_sequencer::SustainStage;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;

static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr stableHighGate = Latch{true, false};
static auto constexpr stableLowGate = Latch{false, false};

using ::testing::Test;

struct SustainStageTest : public Test {
  std::vector<rack::engine::Input> inputs{Controls::InputCount};
  std::vector<rack::engine::Param> params{Controls::ParameterCount};
  std::vector<rack::engine::Light> lights{Controls::LightCount};

  SustainStage<stepCount> sustainStage{inputs, params, lights};

  void givenSustainMode(int step, StageMode mode) {
    params[Controls::SustainModeSwitches + step].setValue(static_cast<float>(mode));
  }
};

TEST_F(SustainStageTest, enter_lightsStepGeneratingLight) {
  auto const step = 4;

  lights[Controls::SustainingLights + step].setBrightness(22.F);

  sustainStage.enter(step);

  EXPECT_EQ(lights[Controls::SustainingLights + step].getBrightness(), 10.F);
}

TEST_F(SustainStageTest, exit_dimsStepGeneratingLight) {
  auto const step = 5;

  sustainStage.enter(step);
  sustainStage.exit();

  EXPECT_EQ(lights[Controls::SustainingLights + step].getBrightness(), 0.F);
}

TEST_F(SustainStageTest, highMode_returnsSustaining_ifGateIsHigh) {
  auto const step = 7;
  givenSustainMode(step, StageMode::High);

  sustainStage.enter(step);

  auto next = sustainStage.execute(risenGate);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = sustainStage.execute(stableHighGate);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}

TEST_F(SustainStageTest, highMode_returnsAdvancing_ifGateIsLow) {
  auto const step = 6;
  givenSustainMode(step, StageMode::High);

  sustainStage.enter(step);

  auto next = sustainStage.execute(fallenGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);

  next = sustainStage.execute(stableLowGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);
}

TEST_F(SustainStageTest, lowMode_returnsSustaining_ifGateIsLow) {
  auto const step = 1;
  givenSustainMode(step, StageMode::Low);

  sustainStage.enter(step);

  auto next = sustainStage.execute(fallenGate);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = sustainStage.execute(stableLowGate);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}

TEST_F(SustainStageTest, lowMode_returnsAdvancing_ifGateIsHigh) {
  auto const step = 0;
  givenSustainMode(step, StageMode::Low);

  sustainStage.enter(step);

  auto next = sustainStage.execute(risenGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);

  next = sustainStage.execute(stableHighGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);
}

TEST_F(SustainStageTest, calmMode_returnsSustaining_ifGateIsStable) {
  auto const step = 5;
  givenSustainMode(step, StageMode::Calm);

  sustainStage.enter(step);

  auto next = sustainStage.execute(stableHighGate);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = sustainStage.execute(stableLowGate);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}

TEST_F(SustainStageTest, calmMode_returnsAdvancing_ifGateChanges) {
  auto const step = 4;
  givenSustainMode(step, StageMode::Calm);

  sustainStage.enter(step);

  auto next = sustainStage.execute(risenGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);

  next = sustainStage.execute(fallenGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);
}

TEST_F(SustainStageTest, skipMode_returnsAdvancing_forEveryGateState) {
  auto const step = 0;
  givenSustainMode(step, StageMode::Skip);

  sustainStage.enter(step);

  auto next = sustainStage.execute(risenGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);

  next = sustainStage.execute(fallenGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);

  next = sustainStage.execute(stableHighGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);

  next = sustainStage.execute(stableLowGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);
}
