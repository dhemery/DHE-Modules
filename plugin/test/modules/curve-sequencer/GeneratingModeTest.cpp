#include "curve-sequencer/GeneratingMode.h"

#include "components/Latch.h"
#include "curve-sequencer/CurveSequencerControls.h"
#include "curve-sequencer/InterruptModes.h"

#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <gtest/gtest.h>

static auto constexpr stepCount{8};

using dhe::Latch;
using dhe::curve_sequencer::GeneratingMode;
using dhe::curve_sequencer::InterruptMode;
using dhe::curve_sequencer::ModeId;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;

static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr stableHighGate = Latch{true, false};
static auto constexpr stableLowGate = Latch{false, false};

using ::testing::Test;

struct GeneratingModeTest : public Test {
  std::vector<rack::engine::Input> inputs{Controls::InputCount};
  std::vector<rack::engine::Param> params{Controls::ParameterCount};
  std::vector<rack::engine::Light> lights{Controls::LightCount};

  GeneratingMode<stepCount> generatingMode{inputs, params, lights};

  void setInterruptMode(int step, InterruptMode mode) {
    params[Controls::GenerateModeSwitches + step].setValue(static_cast<float>(mode));
  }
};

TEST_F(GeneratingModeTest, enter_lightsStepGeneratingLight) {
  auto const step = 4;

  lights[Controls::GeneratingLights + step].setBrightness(22.F);

  generatingMode.enter(step);

  EXPECT_EQ(lights[Controls::GeneratingLights + step].getBrightness(), 10.F);
}

TEST_F(GeneratingModeTest, exit_dimsStepGeneratingLight) {
  auto const step = 5;

  generatingMode.enter(step);
  generatingMode.exit();

  EXPECT_EQ(lights[Controls::GeneratingLights + step].getBrightness(), 0.F);
}

TEST_F(GeneratingModeTest, riseMode_returnsAdvancing_ifGateRises) {
  auto const step = 0;
  setInterruptMode(step, InterruptMode::Rise);

  generatingMode.enter(step);
  auto next = generatingMode.execute(risenGate, 0.F);

  EXPECT_EQ(next.mode, ModeId::Advancing);
}

TEST_F(GeneratingModeTest, riseMode_returnsGenerating_ifGateDoesNotRise) {
  auto const step = 1;
  setInterruptMode(step, InterruptMode::Rise);

  generatingMode.enter(step);

  auto next = generatingMode.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Generating);

  next = generatingMode.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Generating);

  next = generatingMode.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Generating);
}

TEST_F(GeneratingModeTest, fallMode_returnsAdvancing_ifGateFalls) {
  auto const step = 2;
  setInterruptMode(step, InterruptMode::Fall);

  generatingMode.enter(step);

  auto next = generatingMode.execute(fallenGate, 0.F);

  EXPECT_EQ(next.mode, ModeId::Advancing);
}

TEST_F(GeneratingModeTest, fallMode_returnsGenerating_ifGateDoesNotFall) {
  auto const step = 3;
  setInterruptMode(step, InterruptMode::Fall);

  generatingMode.enter(step);

  auto next = generatingMode.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Generating);

  next = generatingMode.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Generating);

  next = generatingMode.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Generating);
}

TEST_F(GeneratingModeTest, edgeMode_returnsAdvancing_ifGateChanges) {
  auto const step = 4;
  setInterruptMode(step, InterruptMode::Edge);

  generatingMode.enter(step);

  auto next = generatingMode.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Advancing);

  next = generatingMode.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Advancing);
}

TEST_F(GeneratingModeTest, edgeMode_returnsGenerating_ifGateDoesNotChange) {
  auto const step = 5;
  setInterruptMode(step, InterruptMode::Edge);

  generatingMode.enter(step);

  auto next = generatingMode.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Generating);

  next = generatingMode.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Generating);
}

TEST_F(GeneratingModeTest, highMode_returnsAdvancing_ifGateIsHigh) {
  auto const step = 6;
  setInterruptMode(step, InterruptMode::High);

  generatingMode.enter(step);

  auto next = generatingMode.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Advancing);

  next = generatingMode.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Advancing);
}

TEST_F(GeneratingModeTest, highMode_returnsGenerating_ifGateIsNotHigh) {
  auto const step = 7;
  setInterruptMode(step, InterruptMode::High);

  generatingMode.enter(step);

  auto next = generatingMode.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Generating);

  next = generatingMode.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Generating);
}

TEST_F(GeneratingModeTest, lowMode_returnsAdvancing_ifGateIsLow) {
  auto const step = 0;
  setInterruptMode(step, InterruptMode::Low);

  generatingMode.enter(step);

  auto next = generatingMode.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Advancing);

  next = generatingMode.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Advancing);
}

TEST_F(GeneratingModeTest, lowMode_returnsGenerating_ifGateIsNotLow) {
  auto const step = 1;
  setInterruptMode(step, InterruptMode::Low);

  generatingMode.enter(step);

  auto next = generatingMode.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Generating);

  next = generatingMode.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Generating);
}

TEST_F(GeneratingModeTest, skipMode_returnsAdvancing_forEveryGateState) {
  auto const step = 0;
  setInterruptMode(step, InterruptMode::Skip);

  generatingMode.enter(step);

  auto next = generatingMode.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Advancing);

  next = generatingMode.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Advancing);

  next = generatingMode.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Advancing);

  next = generatingMode.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, ModeId::Advancing);
}
