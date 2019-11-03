#include "curve-sequencer/SustainingMode.h"

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
using dhe::curve_sequencer::SustainingMode;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;

static auto constexpr ignoredRunLatch = Latch{};
static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr stableHighGate = Latch{true, false};
static auto constexpr stableLowGate = Latch{false, false};

using ::testing::Test;

struct SustainingModeTest : public Test {
  std::vector<rack::engine::Input> inputs{Controls::InputCount};
  std::vector<rack::engine::Param> params{Controls::ParameterCount};
  std::vector<rack::engine::Light> lights{Controls::LightCount};

  SustainingMode<stepCount> sustainingMode{inputs, params, lights};

  void setInterruptMode(int step, InterruptMode mode) {
    params[Controls::SustainModeSwitches + step].setValue(static_cast<float>(mode));
  }
};

TEST_F(SustainingModeTest, isNotTerminal) { ASSERT_EQ(sustainingMode.isTerminal(), false); }

TEST_F(SustainingModeTest, riseMode_returnsAdvancing_ifGateRises) {
  auto const step = 0;
  setInterruptMode(step, InterruptMode::Rise);

  auto next = sustainingMode.execute(ignoredRunLatch, risenGate, step, 0.F);

  EXPECT_EQ(next.modeId, ModeId::Advancing);
}

TEST_F(SustainingModeTest, riseMode_returnsSustaining_ifGateDoesNotRise) {
  auto const step = 1;
  setInterruptMode(step, InterruptMode::Rise);

  auto next = sustainingMode.execute(ignoredRunLatch, fallenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Sustaining);

  next = sustainingMode.execute(ignoredRunLatch, stableHighGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Sustaining);

  next = sustainingMode.execute(ignoredRunLatch, stableLowGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Sustaining);
}

TEST_F(SustainingModeTest, fallMode_returnsAdvancing_ifGateFalls) {
  auto const step = 2;
  setInterruptMode(step, InterruptMode::Fall);

  auto next = sustainingMode.execute(ignoredRunLatch, fallenGate, step, 0.F);

  EXPECT_EQ(next.modeId, ModeId::Advancing);
}

TEST_F(SustainingModeTest, fallMode_returnsSustaining_ifGateDoesNotFall) {
  auto const step = 3;
  setInterruptMode(step, InterruptMode::Fall);

  auto next = sustainingMode.execute(ignoredRunLatch, risenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Sustaining);

  next = sustainingMode.execute(ignoredRunLatch, stableHighGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Sustaining);

  next = sustainingMode.execute(ignoredRunLatch, stableLowGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Sustaining);
}

TEST_F(SustainingModeTest, edgeMode_returnsAdvancing_ifGateChanges) {
  auto const step = 4;
  setInterruptMode(step, InterruptMode::Edge);

  auto next = sustainingMode.execute(ignoredRunLatch, risenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);

  next = sustainingMode.execute(ignoredRunLatch, fallenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);
}

TEST_F(SustainingModeTest, edgeMode_returnsSustaining_ifGateDoesNotChange) {
  auto const step = 5;
  setInterruptMode(step, InterruptMode::Edge);

  auto next = sustainingMode.execute(ignoredRunLatch, stableHighGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Sustaining);

  next = sustainingMode.execute(ignoredRunLatch, stableLowGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Sustaining);
}

TEST_F(SustainingModeTest, highMode_returnsAdvancing_ifGateIsHigh) {
  auto const step = 6;
  setInterruptMode(step, InterruptMode::High);

  auto next = sustainingMode.execute(ignoredRunLatch, risenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);

  next = sustainingMode.execute(ignoredRunLatch, stableHighGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);
}

TEST_F(SustainingModeTest, highMode_returnsSustaining_ifGateIsNotHigh) {
  auto const step = 7;
  setInterruptMode(step, InterruptMode::High);

  auto next = sustainingMode.execute(ignoredRunLatch, fallenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Sustaining);

  next = sustainingMode.execute(ignoredRunLatch, stableLowGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Sustaining);
}

TEST_F(SustainingModeTest, lowMode_returnsAdvancing_ifGateIsLow) {
  auto const step = 0;
  setInterruptMode(step, InterruptMode::Low);

  auto next = sustainingMode.execute(ignoredRunLatch, fallenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);

  next = sustainingMode.execute(ignoredRunLatch, stableLowGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);
}

TEST_F(SustainingModeTest, lowMode_returnsSustaining_ifGateIsNotLow) {
  auto const step = 1;
  setInterruptMode(step, InterruptMode::Low);

  auto next = sustainingMode.execute(ignoredRunLatch, risenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Sustaining);

  next = sustainingMode.execute(ignoredRunLatch, stableHighGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Sustaining);
}

TEST_F(SustainingModeTest, skipMode_returnsAdvancing_forEveryGateState) {
  auto const step = 0;
  setInterruptMode(step, InterruptMode::Skip);

  auto next = sustainingMode.execute(ignoredRunLatch, risenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);

  next = sustainingMode.execute(ignoredRunLatch, fallenGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);

  next = sustainingMode.execute(ignoredRunLatch, stableHighGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);

  next = sustainingMode.execute(ignoredRunLatch, stableLowGate, step, 0.F);
  EXPECT_EQ(next.modeId, ModeId::Advancing);
}
