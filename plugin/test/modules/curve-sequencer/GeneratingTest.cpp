#include "curve-sequencer/Generating.h"

#include "components/Latch.h"
#include "curve-sequencer/CurveSequencerControls.h"
#include "curve-sequencer/SequenceMode.h"
#include "curve-sequencer/StepMode.h"

#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <gtest/gtest.h>

static auto constexpr stepCount{8};

using dhe::Latch;
using dhe::curve_sequencer::Generating;
using dhe::curve_sequencer::SequenceMode;
using dhe::curve_sequencer::StepMode;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;

static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr stableHighGate = Latch{true, false};
static auto constexpr stableLowGate = Latch{false, false};

using ::testing::Test;

struct GeneratingTest : public Test {
  std::vector<rack::engine::Input> inputs{Controls::InputCount};
  std::vector<rack::engine::Param> params{Controls::ParameterCount};
  std::vector<rack::engine::Light> lights{Controls::LightCount};

  Generating<stepCount> generating{inputs, params, lights};

  void setInterruptMode(int step, StepMode mode) {
    params[Controls::GenerateModeSwitches + step].setValue(static_cast<float>(mode));
  }
};

TEST_F(GeneratingTest, enter_lightsStepGeneratingLight) {
  auto const step = 4;

  lights[Controls::GeneratingLights + step].setBrightness(22.F);

  generating.enter(step);

  EXPECT_EQ(lights[Controls::GeneratingLights + step].getBrightness(), 10.F);
}

TEST_F(GeneratingTest, exit_dimsStepGeneratingLight) {
  auto const step = 5;

  generating.enter(step);
  generating.exit();

  EXPECT_EQ(lights[Controls::GeneratingLights + step].getBrightness(), 0.F);
}

TEST_F(GeneratingTest, highMode_returnsGenerating_ifGateIsHigh) {
  auto const step = 7;
  setInterruptMode(step, StepMode::High);

  generating.enter(step);

  auto next = generating.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);

  next = generating.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);
}

TEST_F(GeneratingTest, highMode_returnsSustaining_ifGateIsLow) {
  auto const step = 6;
  setInterruptMode(step, StepMode::High);

  generating.enter(step);

  auto next = generating.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generating.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}

TEST_F(GeneratingTest, lowMode_returnsGenerating_ifGateIsLow) {
  auto const step = 1;
  setInterruptMode(step, StepMode::Low);

  generating.enter(step);

  auto next = generating.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);

  next = generating.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);
}

TEST_F(GeneratingTest, lowMode_returnsSustaining_ifGateIsHigh) {
  auto const step = 0;
  setInterruptMode(step, StepMode::Low);

  generating.enter(step);

  auto next = generating.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generating.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}

TEST_F(GeneratingTest, calmMode_returnsGenerating_ifGateIsStable) {
  auto const step = 5;
  setInterruptMode(step, StepMode::Calm);

  generating.enter(step);

  auto next = generating.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);

  next = generating.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);
}

TEST_F(GeneratingTest, calmMode_returnsSustaining_ifGateChanges) {
  auto const step = 4;
  setInterruptMode(step, StepMode::Calm);

  generating.enter(step);

  auto next = generating.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generating.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}

TEST_F(GeneratingTest, skipMode_returnsSustaining_forEveryGateState) {
  auto const step = 0;
  setInterruptMode(step, StepMode::Skip);

  generating.enter(step);

  auto next = generating.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generating.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generating.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generating.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}
