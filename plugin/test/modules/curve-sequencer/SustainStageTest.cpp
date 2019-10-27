#include "components/Latch.h"
#include "curve-sequencer/Stages.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::SustainStage;

using ::testing::A;
using ::testing::An;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class MockSequenceControls {
public:
  MOCK_METHOD(int, sustainMode, (int), (const));
  MOCK_METHOD(void, setSustaining, (int, bool) );
};

static auto constexpr defaultStep = 3;
static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr stableHighGate = Latch{true, false};
static auto constexpr stableLowGate = Latch{false, false};

class SustainStageTest : public Test {
protected:
  NiceMock<MockSequenceControls> controls;
  SustainStage<MockSequenceControls> sustain{controls};

  void setMode(int mode) { ON_CALL(controls, sustainMode(defaultStep)).WillByDefault(Return(mode)); }
};

class SustainStageRiseMode : public SustainStageTest {
protected:
  void SetUp() override {
    SustainStageTest::SetUp();
    setMode(dhe::curve_sequencer::riseMode);
  }
};

TEST_F(SustainStageRiseMode, isActive_ifGateDoesNotRise) {
  auto const expectedIsActive = true;

  EXPECT_CALL(controls, setSustaining(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, stableHighGate);

  EXPECT_EQ(isActive, expectedIsActive);
}

TEST_F(SustainStageRiseMode, isInactive_ifGateRises) {
  auto const expectedIsActive = false;

  EXPECT_CALL(controls, setSustaining(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, risenGate);

  EXPECT_EQ(isActive, expectedIsActive);
}

class SustainStageFallMode : public SustainStageTest {
protected:
  void SetUp() override {
    SustainStageTest::SetUp();
    setMode(dhe::curve_sequencer::fallMode);
  }
};

TEST_F(SustainStageFallMode, isActive_ifGateDoesNotFall) {
  auto const expectedIsActive = true;

  EXPECT_CALL(controls, setSustaining(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, stableHighGate);

  EXPECT_EQ(isActive, expectedIsActive);
}

TEST_F(SustainStageFallMode, isInactive_ifGateFalls) {
  auto const expectedIsActive = false;

  EXPECT_CALL(controls, setSustaining(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, fallenGate);

  EXPECT_EQ(isActive, expectedIsActive);
}

class SustainStageEdgeMode : public SustainStageTest {
protected:
  void SetUp() override {
    SustainStageTest::SetUp();
    setMode(dhe::curve_sequencer::edgeMode);
  }
};

TEST_F(SustainStageEdgeMode, isActive_ifGateDoesNotChange) {
  auto const expectedIsActive = true;

  EXPECT_CALL(controls, setSustaining(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, stableHighGate);

  EXPECT_EQ(isActive, expectedIsActive);
}

TEST_F(SustainStageEdgeMode, isInactive_ifGateChanges) {
  auto const expectedIsActive = false;

  EXPECT_CALL(controls, setSustaining(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, risenGate);

  EXPECT_EQ(isActive, expectedIsActive);
}

class SustainStageHighMode : public SustainStageTest {
protected:
  void SetUp() override {
    SustainStageTest::SetUp();
    setMode(dhe::curve_sequencer::highMode);
  }
};

TEST_F(SustainStageHighMode, isActive_ifGateIsLow) {
  auto const expectedIsActive = true;

  EXPECT_CALL(controls, setSustaining(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, stableLowGate);

  EXPECT_EQ(isActive, expectedIsActive);
}

TEST_F(SustainStageHighMode, isInactive_ifGateIsHigh) {
  auto const expectedIsActive = false;

  EXPECT_CALL(controls, setSustaining(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, stableHighGate);

  EXPECT_EQ(isActive, expectedIsActive);
}

class SustainStageLowMode : public SustainStageTest {
protected:
  void SetUp() override {
    SustainStageTest::SetUp();
    setMode(dhe::curve_sequencer::lowMode);
  }
};

TEST_F(SustainStageLowMode, isActive_ifGateIsHigh) {
  auto const expectedIsActive = true;

  EXPECT_CALL(controls, setSustaining(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, stableHighGate);

  EXPECT_EQ(isActive, expectedIsActive);
}

TEST_F(SustainStageLowMode, isInactive_ifGateIsLow) {
  auto const expectedIsActive = false;

  EXPECT_CALL(controls, setSustaining(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, stableLowGate);

  EXPECT_EQ(isActive, expectedIsActive);
}

class SustainStageSkipMode : public SustainStageTest {
protected:
  void SetUp() override {
    SustainStageTest::SetUp();
    setMode(dhe::curve_sequencer::skipMode);
  }
};

TEST_F(SustainStageSkipMode, isInactive) {
  auto const expectedIsActive = false;

  EXPECT_CALL(controls, setSustaining(defaultStep, expectedIsActive));

  auto const isActive = sustain.execute(defaultStep, Latch{});

  EXPECT_EQ(isActive, expectedIsActive);
}
