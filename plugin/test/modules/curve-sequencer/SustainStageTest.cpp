#include "curve-sequencer/SustainStage.h"

#include "components/Latch.h"
#include "curve-sequencer/SequenceMode.h"
#include "curve-sequencer/StageMode.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::SequenceMode;
using dhe::curve_sequencer::StageMode;
using dhe::curve_sequencer::SustainStage;

static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr stableHighGate = Latch{true, false};
static auto constexpr stableLowGate = Latch{false, false};

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class MockControls {
public:
  MOCK_METHOD(void, showSustaining, (int, bool) );
  MOCK_METHOD(StageMode, sustainMode, (int), (const));
  MOCK_METHOD(bool, isEnabled, (int), (const));
};

struct SustainStageTest : public Test {
  NiceMock<MockControls> controls{};
  SustainStage<MockControls> sustainStage{controls};

  void givenEnabled(int step, bool state) { ON_CALL(controls, isEnabled(step)).WillByDefault(Return(state)); }
  void givenSustainMode(int step, StageMode mode) { ON_CALL(controls, sustainMode(step)).WillByDefault(Return(mode)); }
};

TEST_F(SustainStageTest, enter_lightsStepGeneratingLight) {
  auto const step = 4;

  EXPECT_CALL(controls, showSustaining(step, true));

  sustainStage.enter(step);
}

TEST_F(SustainStageTest, exit_dimsStepGeneratingLight) {
  auto const step = 5;

  sustainStage.enter(step);

  EXPECT_CALL(controls, showSustaining(step, false));

  sustainStage.exit();
}

TEST_F(SustainStageTest, highMode_isAvailable_ifEnabled_andGateIsHigh) {
  auto constexpr step = 0;
  givenSustainMode(step, StageMode::High);
  givenEnabled(step, true);

  EXPECT_TRUE(sustainStage.isAvailable(step, risenGate));
  EXPECT_TRUE(sustainStage.isAvailable(step, stableHighGate));
}

TEST_F(SustainStageTest, highMode_isUnavailable_ifEnabled_andGateIsLow) {
  auto constexpr step = 1;
  givenSustainMode(step, StageMode::High);
  givenEnabled(step, false);

  EXPECT_FALSE(sustainStage.isAvailable(step, fallenGate));
  EXPECT_FALSE(sustainStage.isAvailable(step, stableLowGate));
}

TEST_F(SustainStageTest, highMode_returnsSustaining_ifGateIsHigh) {
  auto const step = 7;
  givenSustainMode(step, StageMode::High);

  sustainStage.enter(step);

  auto next = sustainStage.execute(risenGate);
  EXPECT_EQ(next, SequenceMode::Sustaining);

  next = sustainStage.execute(stableHighGate);
  EXPECT_EQ(next, SequenceMode::Sustaining);
}

TEST_F(SustainStageTest, highMode_returnsAdvancing_ifGateIsLow) {
  auto const step = 6;
  givenSustainMode(step, StageMode::High);

  sustainStage.enter(step);

  auto next = sustainStage.execute(fallenGate);
  EXPECT_EQ(next, SequenceMode::Advancing);

  next = sustainStage.execute(stableLowGate);
  EXPECT_EQ(next, SequenceMode::Advancing);
}

TEST_F(SustainStageTest, lowMode_isAvailable_ifEnabled_andGateIsLow) {
  auto constexpr step = 0;
  givenSustainMode(step, StageMode::Low);
  givenEnabled(step, true);

  EXPECT_TRUE(sustainStage.isAvailable(step, fallenGate));
  EXPECT_TRUE(sustainStage.isAvailable(step, stableLowGate));
}

TEST_F(SustainStageTest, lowMode_isUnavailable_ifEnabled_andGateIsHigh) {
  auto constexpr step = 1;
  givenSustainMode(step, StageMode::Low);
  givenEnabled(step, true);

  EXPECT_FALSE(sustainStage.isAvailable(step, risenGate));
  EXPECT_FALSE(sustainStage.isAvailable(step, stableHighGate));
}

TEST_F(SustainStageTest, lowMode_returnsSustaining_ifGateIsLow) {
  auto const step = 1;
  givenSustainMode(step, StageMode::Low);

  sustainStage.enter(step);

  auto next = sustainStage.execute(fallenGate);
  EXPECT_EQ(next, SequenceMode::Sustaining);

  next = sustainStage.execute(stableLowGate);
  EXPECT_EQ(next, SequenceMode::Sustaining);
}

TEST_F(SustainStageTest, lowMode_returnsAdvancing_ifGateIsHigh) {
  auto const step = 0;
  givenSustainMode(step, StageMode::Low);

  sustainStage.enter(step);

  auto next = sustainStage.execute(risenGate);
  EXPECT_EQ(next, SequenceMode::Advancing);

  next = sustainStage.execute(stableHighGate);
  EXPECT_EQ(next, SequenceMode::Advancing);
}

TEST_F(SustainStageTest, calmMode_isAvailable_ifEnabled_andGateIsCalm) {
  auto constexpr step = 0;
  givenSustainMode(step, StageMode::Calm);
  givenEnabled(step, true);

  EXPECT_TRUE(sustainStage.isAvailable(step, stableHighGate));
  EXPECT_TRUE(sustainStage.isAvailable(step, stableLowGate));
}

TEST_F(SustainStageTest, calmMode_isUnavailable_ifEnabled_andGateIsEdge) {
  auto constexpr step = 1;
  givenSustainMode(step, StageMode::Calm);
  givenEnabled(step, true);

  EXPECT_FALSE(sustainStage.isAvailable(step, risenGate));
  EXPECT_FALSE(sustainStage.isAvailable(step, fallenGate));
}

TEST_F(SustainStageTest, calmMode_returnsSustaining_ifGateIsStable) {
  auto const step = 5;
  givenSustainMode(step, StageMode::Calm);

  sustainStage.enter(step);

  auto next = sustainStage.execute(stableHighGate);
  EXPECT_EQ(next, SequenceMode::Sustaining);

  next = sustainStage.execute(stableLowGate);
  EXPECT_EQ(next, SequenceMode::Sustaining);
}

TEST_F(SustainStageTest, calmMode_returnsAdvancing_ifGateChanges) {
  auto const step = 4;
  givenSustainMode(step, StageMode::Calm);

  sustainStage.enter(step);

  auto next = sustainStage.execute(risenGate);
  EXPECT_EQ(next, SequenceMode::Advancing);

  next = sustainStage.execute(fallenGate);
  EXPECT_EQ(next, SequenceMode::Advancing);
}

TEST_F(SustainStageTest, skipMode_isUnavailable_regardlessOfEnablementAndGate) {
  auto constexpr step = 1;
  givenSustainMode(step, StageMode::Skip);

  givenEnabled(step, true);
  EXPECT_FALSE(sustainStage.isAvailable(step, stableHighGate));
  EXPECT_FALSE(sustainStage.isAvailable(step, stableLowGate));
  EXPECT_FALSE(sustainStage.isAvailable(step, risenGate));
  EXPECT_FALSE(sustainStage.isAvailable(step, fallenGate));

  givenEnabled(step, false);
  EXPECT_FALSE(sustainStage.isAvailable(step, stableHighGate));
  EXPECT_FALSE(sustainStage.isAvailable(step, stableLowGate));
  EXPECT_FALSE(sustainStage.isAvailable(step, risenGate));
  EXPECT_FALSE(sustainStage.isAvailable(step, fallenGate));
}

TEST_F(SustainStageTest, skipMode_returnsAdvancing_forEveryGateState) {
  auto const step = 0;
  givenSustainMode(step, StageMode::Skip);

  sustainStage.enter(step);

  auto next = sustainStage.execute(risenGate);
  EXPECT_EQ(next, SequenceMode::Advancing);

  next = sustainStage.execute(fallenGate);
  EXPECT_EQ(next, SequenceMode::Advancing);

  next = sustainStage.execute(stableHighGate);
  EXPECT_EQ(next, SequenceMode::Advancing);

  next = sustainStage.execute(stableLowGate);
  EXPECT_EQ(next, SequenceMode::Advancing);
}

TEST_F(SustainStageTest, ifDisabled_isUnavailable_regardlessOfModeAndGate) {
  auto constexpr step = 5;
  givenEnabled(step, false);

  givenSustainMode(step, StageMode::High);
  EXPECT_FALSE(sustainStage.isAvailable(step, stableHighGate));
  EXPECT_FALSE(sustainStage.isAvailable(step, risenGate));

  givenSustainMode(step, StageMode::Low);
  EXPECT_FALSE(sustainStage.isAvailable(step, stableLowGate));
  EXPECT_FALSE(sustainStage.isAvailable(step, fallenGate));

  givenSustainMode(step, StageMode::Calm);
  EXPECT_FALSE(sustainStage.isAvailable(step, stableLowGate));
  EXPECT_FALSE(sustainStage.isAvailable(step, stableHighGate));
}
