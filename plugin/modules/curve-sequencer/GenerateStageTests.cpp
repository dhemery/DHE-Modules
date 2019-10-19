#include "Stages.h"
#include "components/Latch.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::GenerateStage;

using ::testing::A;
using ::testing::An;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class MockSequenceControls {
public:
  MOCK_METHOD(int, generateMode, (int), (const));
  MOCK_METHOD(void, setGenerating, (int, bool) );
};

static auto constexpr defaultStep = 3;
static auto constexpr defaultSampleTime = 0.F;

static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr stableHighGate = Latch{true, false};
static auto constexpr stableLowGate = Latch{false, false};

class GenerateStageTest : public Test {
protected:
  NiceMock<MockSequenceControls> controls;
  GenerateStage<MockSequenceControls> sustain{controls};

  void setMode(int mode) { ON_CALL(controls, generateMode(defaultStep)).WillByDefault(Return(mode)); }
};

class GenerateStageRiseMode : public GenerateStageTest {
protected:
  void SetUp() override {
    GenerateStageTest::SetUp();
    setMode(dhe::curve_sequencer::riseMode);
  }
};

TEST_F(GenerateStageRiseMode, isActive_ifGateDoesNotRise) {
  auto const expectedIsActive = true;

  EXPECT_CALL(controls, setGenerating(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, expectedIsActive);
}

TEST_F(GenerateStageRiseMode, isInactive_ifGateRises) {
  auto const expectedIsActive = false;

  EXPECT_CALL(controls, setGenerating(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, risenGate, defaultSampleTime);

  EXPECT_EQ(isActive, expectedIsActive);
}

class GenerateStageFallMode : public GenerateStageTest {
protected:
  void SetUp() override {
    GenerateStageTest::SetUp();
    setMode(dhe::curve_sequencer::fallMode);
  }
};

TEST_F(GenerateStageFallMode, isActive_ifGateDoesNotFall) {
  auto const expectedIsActive = true;

  EXPECT_CALL(controls, setGenerating(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, expectedIsActive);
}

TEST_F(GenerateStageFallMode, isInactive_ifGateFalls) {
  auto const expectedIsActive = false;

  EXPECT_CALL(controls, setGenerating(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, fallenGate, defaultSampleTime);

  EXPECT_EQ(isActive, expectedIsActive);
}

class GenerateStageEdgeMode : public GenerateStageTest {
protected:
  void SetUp() override {
    GenerateStageTest::SetUp();
    setMode(dhe::curve_sequencer::edgeMode);
  }
};

TEST_F(GenerateStageEdgeMode, isActive_ifGateDoesNotChange) {
  auto const expectedIsActive = true;

  EXPECT_CALL(controls, setGenerating(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, expectedIsActive);
}

TEST_F(GenerateStageEdgeMode, isInactive_ifGateChanges) {
  auto const expectedIsActive = false;

  EXPECT_CALL(controls, setGenerating(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, risenGate, defaultSampleTime);

  EXPECT_EQ(isActive, expectedIsActive);
}

class GenerateStageHighMode : public GenerateStageTest {
protected:
  void SetUp() override {
    GenerateStageTest::SetUp();
    setMode(dhe::curve_sequencer::highMode);
  }
};

TEST_F(GenerateStageHighMode, isActive_ifGateIsLow) {
  auto const expectedIsActive = true;

  EXPECT_CALL(controls, setGenerating(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, stableLowGate, defaultSampleTime);

  EXPECT_EQ(isActive, expectedIsActive);
}

TEST_F(GenerateStageHighMode, isInactive_ifGateIsHigh) {
  auto const expectedIsActive = false;

  EXPECT_CALL(controls, setGenerating(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, expectedIsActive);
}

class GenerateStageLowMode : public GenerateStageTest {
protected:
  void SetUp() override {
    GenerateStageTest::SetUp();
    setMode(dhe::curve_sequencer::lowMode);
  }
};

TEST_F(GenerateStageLowMode, isActive_ifGateIsHigh) {
  auto const expectedIsActive = true;

  EXPECT_CALL(controls, setGenerating(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, expectedIsActive);
}

TEST_F(GenerateStageLowMode, isInactive_ifGateIsLow) {
  auto const expectedIsActive = false;

  EXPECT_CALL(controls, setGenerating(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, stableLowGate, defaultSampleTime);

  EXPECT_EQ(isActive, expectedIsActive);
}

class GenerateStageSkipMode : public GenerateStageTest {
protected:
  void SetUp() override {
    GenerateStageTest::SetUp();
    setMode(dhe::curve_sequencer::skipMode);
  }
};

TEST_F(GenerateStageSkipMode, isInactive) {
  auto const expectedIsActive = false;

  EXPECT_CALL(controls, setGenerating(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, Latch{}, defaultSampleTime);

  EXPECT_EQ(isActive, expectedIsActive);
}

class GenerateStageDurationMode : public GenerateStageTest {
protected:
  void SetUp() override {
    GenerateStageTest::SetUp();
    setMode(dhe::curve_sequencer::durationMode);
  }
};

TEST_F(GenerateStageDurationMode, isActive) {
  auto const expectedIsActive = true;

  EXPECT_CALL(controls, setGenerating(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, stableHighGate, defaultSampleTime);

  EXPECT_EQ(isActive, expectedIsActive);
}
