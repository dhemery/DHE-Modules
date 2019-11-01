#include "components/Latch.h"
#include "curve-sequencer/Stages.h"

#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <gtest/gtest.h>

static auto constexpr stepCount{4};

using dhe::Latch;
using dhe::curve_sequencer::SustainStage;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;

using ::testing::Test;

using InputType = rack::engine::Input;
using OutputType = rack::engine::Output;
using ParamType = rack::engine::Param;
using LightType = rack::engine::Light;

static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr stableHighGate = Latch{true, false};
static auto constexpr stableLowGate = Latch{false, false};

class SustainStageTest : public Test {
protected:
  std::vector<InputType> inputs{Controls::InputCount};
  std::vector<OutputType> outputs{Controls::OutputCount};
  std::vector<ParamType> params{Controls::ParameterCount};
  std::vector<LightType> lights{Controls::LightCount};
  SustainStage<stepCount, InputType, OutputType, ParamType, LightType> sustain{inputs, outputs, params, lights};

  void setMode(int step, int mode) { params[Controls::SustainModeSwitches + step].setValue(static_cast<float>(mode)); }

  auto sustainingLight(int step) -> float { return lights[Controls::SustainingLights + step].getBrightness(); }
};

class SustainStageRiseMode : public SustainStageTest {
protected:
  void SetUp() override { SustainStageTest::SetUp(); }
};

TEST_F(SustainStageRiseMode, isActive_ifGateDoesNotRise) {
  auto const step{0};
  setMode(step, dhe::curve_sequencer::riseMode);

  auto const isActive = sustain.execute(step, stableHighGate);

  EXPECT_EQ(isActive, true);
  EXPECT_EQ(sustainingLight(step), 10.F);
}

TEST_F(SustainStageRiseMode, isInactive_ifGateRises) {
  auto const step{1};
  setMode(step, dhe::curve_sequencer::riseMode);

  auto const isActive = sustain.execute(step, risenGate);

  EXPECT_EQ(isActive, false);
  EXPECT_EQ(sustainingLight(step), 0.F);
}

class SustainStageFallMode : public SustainStageTest {
protected:
  void SetUp() override { SustainStageTest::SetUp(); }
};

TEST_F(SustainStageFallMode, isActive_ifGateDoesNotFall) {
  auto const step{2};
  setMode(step, dhe::curve_sequencer::fallMode);

  auto const isActive = sustain.execute(step, stableHighGate);

  EXPECT_EQ(isActive, true);
  EXPECT_EQ(sustainingLight(step), 10.F);
}

TEST_F(SustainStageFallMode, isInactive_ifGateFalls) {
  auto const step{3};
  setMode(step, dhe::curve_sequencer::fallMode);

  auto const isActive = sustain.execute(step, fallenGate);

  EXPECT_EQ(isActive, false);
  EXPECT_EQ(sustainingLight(step), 0.F);
}

class SustainStageEdgeMode : public SustainStageTest {
protected:
  void SetUp() override { SustainStageTest::SetUp(); }
};

TEST_F(SustainStageEdgeMode, isActive_ifGateDoesNotChange) {
  auto const step{0};
  setMode(step, dhe::curve_sequencer::edgeMode);

  auto const isActive = sustain.execute(step, stableHighGate);

  EXPECT_EQ(isActive, true);
  EXPECT_EQ(sustainingLight(step), 10.F);
}

TEST_F(SustainStageEdgeMode, isInactive_ifGateChanges) {
  auto const step{1};
  setMode(step, dhe::curve_sequencer::edgeMode);

  auto const isActive = sustain.execute(step, risenGate);

  EXPECT_EQ(isActive, false);
  EXPECT_EQ(sustainingLight(step), 0.F);
}

class SustainStageHighMode : public SustainStageTest {
protected:
  void SetUp() override { SustainStageTest::SetUp(); }
};

TEST_F(SustainStageHighMode, isActive_ifGateIsLow) {
  auto const step{2};
  setMode(step, dhe::curve_sequencer::highMode);

  auto const isActive = sustain.execute(step, stableLowGate);

  EXPECT_EQ(isActive, true);
  EXPECT_EQ(sustainingLight(step), 10.F);
}

TEST_F(SustainStageHighMode, isInactive_ifGateIsHigh) {
  auto const step{3};
  setMode(step, dhe::curve_sequencer::highMode);

  auto const isActive = sustain.execute(step, stableHighGate);

  EXPECT_EQ(isActive, false);
  EXPECT_EQ(sustainingLight(step), 0.F);
}

class SustainStageLowMode : public SustainStageTest {
protected:
  void SetUp() override { SustainStageTest::SetUp(); }
};

TEST_F(SustainStageLowMode, isActive_ifGateIsHigh) {
  auto const step{0};
  setMode(step, dhe::curve_sequencer::lowMode);

  auto const isActive = sustain.execute(step, stableHighGate);

  EXPECT_EQ(isActive, true);
  EXPECT_EQ(sustainingLight(step), 10.F);
}

TEST_F(SustainStageLowMode, isInactive_ifGateIsLow) {
  auto const step{1};
  setMode(step, dhe::curve_sequencer::lowMode);

  auto const isActive = sustain.execute(step, stableLowGate);

  EXPECT_EQ(isActive, false);
  EXPECT_EQ(sustainingLight(step), 0.F);
}

class SustainStageSkipMode : public SustainStageTest {
protected:
  void SetUp() override { SustainStageTest::SetUp(); }
};

TEST_F(SustainStageSkipMode, isInactive) {
  auto const step{2};
  setMode(step, dhe::curve_sequencer::skipMode);

  auto const isActive = sustain.execute(step, Latch{});

  EXPECT_EQ(isActive, false);
  EXPECT_EQ(sustainingLight(step), 0.F);
};
