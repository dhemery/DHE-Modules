#include "curve-sequencer/Generating.h"

#include "components/Latch.h"
#include "curve-sequencer/CurveSequencerControls.h"
#include "curve-sequencer/RunModes.h"

#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <gtest/gtest.h>

static auto constexpr stepCount{8};

using dhe::Latch;
using dhe::curve_sequencer::Generating;
using dhe::curve_sequencer::Mode;
using dhe::curve_sequencer::RunMode;
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

  Generating<stepCount> generatingMode{inputs, params, lights};

  void setInterruptMode(int step, RunMode mode) {
    params[Controls::GenerateModeSwitches + step].setValue(static_cast<float>(mode));
  }
};

TEST_F(GeneratingTest, enter_lightsStepGeneratingLight) {
  auto const step = 4;

  lights[Controls::GeneratingLights + step].setBrightness(22.F);

  generatingMode.enter(step);

  EXPECT_EQ(lights[Controls::GeneratingLights + step].getBrightness(), 10.F);
}

TEST_F(GeneratingTest, exit_dimsStepGeneratingLight) {
  auto const step = 5;

  generatingMode.enter(step);
  generatingMode.exit();

  EXPECT_EQ(lights[Controls::GeneratingLights + step].getBrightness(), 0.F);
}

TEST_F(GeneratingTest, highMode_returnsGenerating_ifGateIsHigh) {
  auto const step = 7;
  setInterruptMode(step, RunMode::High);

  generatingMode.enter(step);

  auto next = generatingMode.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Generating);

  next = generatingMode.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Generating);
}

TEST_F(GeneratingTest, highMode_returnsSustaining_ifGateIsLow) {
  auto const step = 6;
  setInterruptMode(step, RunMode::High);

  generatingMode.enter(step);

  auto next = generatingMode.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Sustaining);

  next = generatingMode.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Sustaining);
}

TEST_F(GeneratingTest, lowMode_returnsGenerating_ifGateIsLow) {
  auto const step = 1;
  setInterruptMode(step, RunMode::Low);

  generatingMode.enter(step);

  auto next = generatingMode.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Generating);

  next = generatingMode.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Generating);
}

TEST_F(GeneratingTest, lowMode_returnsSustaining_ifGateIsHigh) {
  auto const step = 0;
  setInterruptMode(step, RunMode::Low);

  generatingMode.enter(step);

  auto next = generatingMode.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Sustaining);

  next = generatingMode.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Sustaining);
}

TEST_F(GeneratingTest, calmMode_returnsGenerating_ifGateIsStable) {
  auto const step = 5;
  setInterruptMode(step, RunMode::Calm);

  generatingMode.enter(step);

  auto next = generatingMode.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Generating);

  next = generatingMode.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Generating);
}

TEST_F(GeneratingTest, calmMode_returnsSustaining_ifGateChanges) {
  auto const step = 4;
  setInterruptMode(step, RunMode::Calm);

  generatingMode.enter(step);

  auto next = generatingMode.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Sustaining);

  next = generatingMode.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Sustaining);
}

TEST_F(GeneratingTest, skipMode_returnsSustaining_forEveryGateState) {
  auto const step = 0;
  setInterruptMode(step, RunMode::Skip);

  generatingMode.enter(step);

  auto next = generatingMode.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Sustaining);

  next = generatingMode.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Sustaining);

  next = generatingMode.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Sustaining);

  next = generatingMode.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, Mode::Sustaining);
}
