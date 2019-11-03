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
using dhe::curve_sequencer::InterruptMode;
using dhe::curve_sequencer::ModeId;
using dhe::curve_sequencer::GeneratingMode;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;

static auto constexpr ignoredRunLatch = Latch{};
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
    params[Controls::SustainModeSwitches + step].setValue(static_cast<float>(mode));
  }
};

TEST_F(GeneratingModeTest, isNotTerminal) { ASSERT_EQ(generatingMode.isTerminal(), false); }

TEST_F(GeneratingModeTest, riseMode_returnsAdvancing_ifGateRises) {
  auto const step = 0;
  setInterruptMode(step, InterruptMode::Rise);

  auto next = generatingMode.execute(ignoredRunLatch, risenGate, step, 0.F);

  EXPECT_EQ(next.modeId, ModeId::Advancing);
}

TEST_F(GeneratingModeTest, riseMode_returnsGenerating_ifGateDoesNotRise) {
  auto const step = 1;
  setInterruptMode(step, InterruptMode::Rise);

  auto next = generatingMode.execute(ignoredRunLatch, fallenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Generating);

  next = generatingMode.execute(ignoredRunLatch, stableHighGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Generating);

  next = generatingMode.execute(ignoredRunLatch, stableLowGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Generating);
}

TEST_F(GeneratingModeTest, fallMode_returnsAdvancing_ifGateFalls) {
  auto const step = 2;
  setInterruptMode(step, InterruptMode::Fall);

  auto next = generatingMode.execute(ignoredRunLatch, fallenGate, step, 0.F);

  EXPECT_EQ(next.modeId, ModeId::Advancing);
}

TEST_F(GeneratingModeTest, fallMode_returnsGenerating_ifGateDoesNotFall) {
  auto const step = 3;
  setInterruptMode(step, InterruptMode::Fall);

  auto next = generatingMode.execute(ignoredRunLatch, risenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Generating);

  next = generatingMode.execute(ignoredRunLatch, stableHighGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Generating);

  next = generatingMode.execute(ignoredRunLatch, stableLowGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Generating);
}

TEST_F(GeneratingModeTest, edgeMode_returnsAdvancing_ifGateChanges) {
  auto const step = 4;
  setInterruptMode(step, InterruptMode::Edge);

  auto next = generatingMode.execute(ignoredRunLatch, risenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);

  next = generatingMode.execute(ignoredRunLatch, fallenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);
}

TEST_F(GeneratingModeTest, edgeMode_returnsGenerating_ifGateDoesNotChange) {
  auto const step = 5;
  setInterruptMode(step, InterruptMode::Edge);

  auto next = generatingMode.execute(ignoredRunLatch, stableHighGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Generating);

  next = generatingMode.execute(ignoredRunLatch, stableLowGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Generating);
}

TEST_F(GeneratingModeTest, highMode_returnsAdvancing_ifGateIsHigh) {
  auto const step = 6;
  setInterruptMode(step, InterruptMode::High);

  auto next = generatingMode.execute(ignoredRunLatch, risenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);

  next = generatingMode.execute(ignoredRunLatch, stableHighGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);
}

TEST_F(GeneratingModeTest, highMode_returnsGenerating_ifGateIsNotHigh) {
  auto const step = 7;
  setInterruptMode(step, InterruptMode::High);

  auto next = generatingMode.execute(ignoredRunLatch, fallenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Generating);

  next = generatingMode.execute(ignoredRunLatch, stableLowGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Generating);
}

TEST_F(GeneratingModeTest, lowMode_returnsAdvancing_ifGateIsLow) {
  auto const step = 0;
  setInterruptMode(step, InterruptMode::Low);

  auto next = generatingMode.execute(ignoredRunLatch, fallenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);

  next = generatingMode.execute(ignoredRunLatch, stableLowGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);
}

TEST_F(GeneratingModeTest, lowMode_returnsGenerating_ifGateIsNotLow) {
  auto const step = 1;
  setInterruptMode(step, InterruptMode::Low);

  auto next = generatingMode.execute(ignoredRunLatch, risenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Generating);

  next = generatingMode.execute(ignoredRunLatch, stableHighGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Generating);
}

TEST_F(GeneratingModeTest, skipMode_returnsAdvancing_forEveryGateState) {
  auto const step = 0;
  setInterruptMode(step, InterruptMode::Skip);

  auto next = generatingMode.execute(ignoredRunLatch, risenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);

  next = generatingMode.execute(ignoredRunLatch, fallenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);

  next = generatingMode.execute(ignoredRunLatch, stableHighGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);

  next = generatingMode.execute(ignoredRunLatch, stableLowGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);
}
