#include "components/Latch.h"
#include "curve-sequencer/Stages.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mocks/MockLight.h>
#include <mocks/MockParam.h>
#include <mocks/MockPort.h>

static auto constexpr step{2};
static auto constexpr stepCount{4};

using dhe::Latch;
using dhe::curve_sequencer::SustainStage;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;

using ::testing::A;
using ::testing::An;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

using InputType = NiceMock<MockPort>;
using OutputType = NiceMock<MockPort>;
using ParamType = NiceMock<MockParam>;
using LightType = NiceMock<MockLight>;

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

  void setMode(int mode) {
    ON_CALL(params[Controls::SustainModeSwitches + step], getValue()).WillByDefault(Return(mode));
  }

  auto sustainingLight() -> LightType & { return lights[Controls::SustainingLights + step]; }
};

class SustainStageRiseMode : public SustainStageTest {
protected:
  void SetUp() override {
    SustainStageTest::SetUp();
    setMode(dhe::curve_sequencer::riseMode);
  }
};

TEST_F(SustainStageRiseMode, isActive_ifGateDoesNotRise) {
  EXPECT_CALL(sustainingLight(), setBrightness(10.F));

  auto const isActive = sustain.execute(step, stableHighGate);

  EXPECT_EQ(isActive, true);
}

TEST_F(SustainStageRiseMode, isInactive_ifGateRises) {
  EXPECT_CALL(sustainingLight(), setBrightness(0.F));

  auto const isActive = sustain.execute(step, risenGate);

  EXPECT_EQ(isActive, false);
}

class SustainStageFallMode : public SustainStageTest {
protected:
  void SetUp() override {
    SustainStageTest::SetUp();
    setMode(dhe::curve_sequencer::fallMode);
  }
};

TEST_F(SustainStageFallMode, isActive_ifGateDoesNotFall) {
  EXPECT_CALL(sustainingLight(), setBrightness(10.F));

  auto const isActive = sustain.execute(step, stableHighGate);

  EXPECT_EQ(isActive, true);
}

TEST_F(SustainStageFallMode, isInactive_ifGateFalls) {
  EXPECT_CALL(sustainingLight(), setBrightness(0.F));

  auto const isActive = sustain.execute(step, fallenGate);

  EXPECT_EQ(isActive, false);
}

class SustainStageEdgeMode : public SustainStageTest {
protected:
  void SetUp() override {
    SustainStageTest::SetUp();
    setMode(dhe::curve_sequencer::edgeMode);
  }
};

TEST_F(SustainStageEdgeMode, isActive_ifGateDoesNotChange) {
  EXPECT_CALL(sustainingLight(), setBrightness(10.F));

  auto const isActive = sustain.execute(step, stableHighGate);

  EXPECT_EQ(isActive, true);
}

TEST_F(SustainStageEdgeMode, isInactive_ifGateChanges) {
  EXPECT_CALL(sustainingLight(), setBrightness(0.F));

  auto const isActive = sustain.execute(step, risenGate);

  EXPECT_EQ(isActive, false);
}

class SustainStageHighMode : public SustainStageTest {
protected:
  void SetUp() override {
    SustainStageTest::SetUp();
    setMode(dhe::curve_sequencer::highMode);
  }
};

TEST_F(SustainStageHighMode, isActive_ifGateIsLow) {
  EXPECT_CALL(sustainingLight(), setBrightness(10.F));

  auto const isActive = sustain.execute(step, stableLowGate);

  EXPECT_EQ(isActive, true);
}

TEST_F(SustainStageHighMode, isInactive_ifGateIsHigh) {
  EXPECT_CALL(sustainingLight(), setBrightness(0.F));

  auto const isActive = sustain.execute(step, stableHighGate);

  EXPECT_EQ(isActive, false);
}

class SustainStageLowMode : public SustainStageTest {
protected:
  void SetUp() override {
    SustainStageTest::SetUp();
    setMode(dhe::curve_sequencer::lowMode);
  }
};

TEST_F(SustainStageLowMode, isActive_ifGateIsHigh) {
  EXPECT_CALL(sustainingLight(), setBrightness(10.F));

  auto const isActive = sustain.execute(step, stableHighGate);

  EXPECT_EQ(isActive, true);
}

TEST_F(SustainStageLowMode, isInactive_ifGateIsLow) {
  EXPECT_CALL(sustainingLight(), setBrightness(0.F));

  auto const isActive = sustain.execute(step, stableLowGate);

  EXPECT_EQ(isActive, false);
}

class SustainStageSkipMode : public SustainStageTest {
protected:
  void SetUp() override {
    SustainStageTest::SetUp();
    setMode(dhe::curve_sequencer::skipMode);
  }
};

TEST_F(SustainStageSkipMode, isInactive) {
  EXPECT_CALL(sustainingLight(), setBrightness(0.F));

  auto const isActive = sustain.execute(step, Latch{});

  EXPECT_EQ(isActive, false);
}
