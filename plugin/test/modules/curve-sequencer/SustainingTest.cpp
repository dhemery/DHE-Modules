#include "curve-sequencer/Sustaining.h"

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
using dhe::curve_sequencer::SequenceMode;
using dhe::curve_sequencer::StepMode;
using dhe::curve_sequencer::Sustaining;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;

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

  void setInterruptMode(int step, StepMode mode) {
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

TEST_F(SustainingTest, highMode_returnsSustaining_ifGateIsHigh) {
  auto const step = 7;
  setInterruptMode(step, StepMode::High);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(risenGate);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = sustainingMode.execute(stableHighGate);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}

TEST_F(SustainingTest, highMode_returnsAdvancing_ifGateIsLow) {
  auto const step = 6;
  setInterruptMode(step, StepMode::High);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(fallenGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);

  next = sustainingMode.execute(stableLowGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);
}

TEST_F(SustainingTest, lowMode_returnsSustaining_ifGateIsLow) {
  auto const step = 1;
  setInterruptMode(step, StepMode::Low);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(fallenGate);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = sustainingMode.execute(stableLowGate);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}

TEST_F(SustainingTest, lowMode_returnsAdvancing_ifGateIsHigh) {
  auto const step = 0;
  setInterruptMode(step, StepMode::Low);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(risenGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);

  next = sustainingMode.execute(stableHighGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);
}

TEST_F(SustainingTest, calmMode_returnsSustaining_ifGateIsStable) {
  auto const step = 5;
  setInterruptMode(step, StepMode::Calm);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(stableHighGate);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = sustainingMode.execute(stableLowGate);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}

TEST_F(SustainingTest, calmMode_returnsAdvancing_ifGateChanges) {
  auto const step = 4;
  setInterruptMode(step, StepMode::Calm);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(risenGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);

  next = sustainingMode.execute(fallenGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);
}

TEST_F(SustainingTest, skipMode_returnsAdvancing_forEveryGateState) {
  auto const step = 0;
  setInterruptMode(step, StepMode::Skip);

  sustainingMode.enter(step);

  auto next = sustainingMode.execute(risenGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);

  next = sustainingMode.execute(fallenGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);

  next = sustainingMode.execute(stableHighGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);

  next = sustainingMode.execute(stableLowGate);
  EXPECT_EQ(next.mode, SequenceMode::Advancing);
}
