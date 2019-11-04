#include "curve-sequencer/Sustaining.h"

#include "components/Latch.h"
#include "curve-sequencer/Controls.h"
#include "curve-sequencer/InterruptModes.h"

#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <gtest/gtest.h>

static auto constexpr stepCount{8};

using dhe::Latch;
using dhe::curve_sequencer::InterruptMode;
using dhe::curve_sequencer::Mode;
using dhe::curve_sequencer::Sustaining;
using Controls = dhe::curve_sequencer::Controls<stepCount>;

static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr stableHighGate = Latch{true, false};
static auto constexpr stableLowGate = Latch{false, false};

using ::testing::Test;

struct SustainingTest : public Test {
  std::vector<rack::engine::Input> inputs{Controls::InputCount};
  std::vector<rack::engine::Param> params{Controls::ParameterCount};
  std::vector<rack::engine::Light> lights{Controls::LightCount};

  Sustaining<stepCount> sustainingMode{inputs, params, lights};

  void setInterruptMode(int step, InterruptMode mode) {
    params[Controls::SustainModeSwitches + step].setValue(static_cast<float>(mode));
  }
};

TEST_F(SustainingTest, enter_lightsStepGeneratingLight) {
  auto const step = 4;

  lights[Controls::SustainingLights + step].setBrightness(22.F);

  sustainingMode.enter(step);

  EXPECT_EQ(lights[Controls::SustainingLights + step].getBrightness(), 10.F);
}

TEST_F(SustainingTest, exit_dimsStepGeneratingLight) {
  auto const step = 5;

  sustainingMode.enter(step);
  sustainingMode.exit();

  EXPECT_EQ(lights[Controls::SustainingLights + step].getBrightness(), 0.F);
}

TEST_F(SustainingTest, riseMode_returnsAdvancing_ifGateRises) {
  auto const step = 0;
  setInterruptMode(step, InterruptMode::Rise);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(risenGate);

  EXPECT_EQ(next.mode, Mode::Advancing);
}

TEST_F(SustainingTest, riseMode_returnsSustaining_ifGateDoesNotRise) {
  auto const step = 1;
  setInterruptMode(step, InterruptMode::Rise);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(fallenGate);
  EXPECT_EQ(next.mode, Mode::Sustaining);

  next = sustainingMode.execute(stableHighGate);
  EXPECT_EQ(next.mode, Mode::Sustaining);

  next = sustainingMode.execute(stableLowGate);
  EXPECT_EQ(next.mode, Mode::Sustaining);
}

TEST_F(SustainingTest, fallMode_returnsAdvancing_ifGateFalls) {
  auto const step = 2;
  setInterruptMode(step, InterruptMode::Fall);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(fallenGate);

  EXPECT_EQ(next.mode, Mode::Advancing);
}

TEST_F(SustainingTest, fallMode_returnsSustaining_ifGateDoesNotFall) {
  auto const step = 3;
  setInterruptMode(step, InterruptMode::Fall);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(risenGate);
  EXPECT_EQ(next.mode, Mode::Sustaining);

  next = sustainingMode.execute(stableHighGate);
  EXPECT_EQ(next.mode, Mode::Sustaining);

  next = sustainingMode.execute(stableLowGate);
  EXPECT_EQ(next.mode, Mode::Sustaining);
}

TEST_F(SustainingTest, edgeMode_returnsAdvancing_ifGateChanges) {
  auto const step = 4;
  setInterruptMode(step, InterruptMode::Edge);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(risenGate);
  EXPECT_EQ(next.mode, Mode::Advancing);

  next = sustainingMode.execute(fallenGate);
  EXPECT_EQ(next.mode, Mode::Advancing);
}

TEST_F(SustainingTest, edgeMode_returnsSustaining_ifGateDoesNotChange) {
  auto const step = 5;
  setInterruptMode(step, InterruptMode::Edge);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(stableHighGate);
  EXPECT_EQ(next.mode, Mode::Sustaining);

  next = sustainingMode.execute(stableLowGate);
  EXPECT_EQ(next.mode, Mode::Sustaining);
}

TEST_F(SustainingTest, highMode_returnsAdvancing_ifGateIsHigh) {
  auto const step = 6;
  setInterruptMode(step, InterruptMode::High);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(risenGate);
  EXPECT_EQ(next.mode, Mode::Advancing);

  next = sustainingMode.execute(stableHighGate);
  EXPECT_EQ(next.mode, Mode::Advancing);
}

TEST_F(SustainingTest, highMode_returnsSustaining_ifGateIsNotHigh) {
  auto const step = 7;
  setInterruptMode(step, InterruptMode::High);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(fallenGate);
  EXPECT_EQ(next.mode, Mode::Sustaining);

  next = sustainingMode.execute(stableLowGate);
  EXPECT_EQ(next.mode, Mode::Sustaining);
}

TEST_F(SustainingTest, lowMode_returnsAdvancing_ifGateIsLow) {
  auto const step = 0;
  setInterruptMode(step, InterruptMode::Low);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(fallenGate);
  EXPECT_EQ(next.mode, Mode::Advancing);

  next = sustainingMode.execute(stableLowGate);
  EXPECT_EQ(next.mode, Mode::Advancing);
}

TEST_F(SustainingTest, lowMode_returnsSustaining_ifGateIsNotLow) {
  auto const step = 1;
  setInterruptMode(step, InterruptMode::Low);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(risenGate);
  EXPECT_EQ(next.mode, Mode::Sustaining);

  next = sustainingMode.execute(stableHighGate);
  EXPECT_EQ(next.mode, Mode::Sustaining);
}

TEST_F(SustainingTest, skipMode_returnsAdvancing_forEveryGateState) {
  auto const step = 0;
  setInterruptMode(step, InterruptMode::Skip);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(risenGate);
  EXPECT_EQ(next.mode, Mode::Advancing);

  next = sustainingMode.execute(fallenGate);
  EXPECT_EQ(next.mode, Mode::Advancing);

  next = sustainingMode.execute(stableHighGate);
  EXPECT_EQ(next.mode, Mode::Advancing);

  next = sustainingMode.execute(stableLowGate);
  EXPECT_EQ(next.mode, Mode::Advancing);
}
