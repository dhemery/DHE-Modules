#include "curve-sequencer/GenerateStage.h"

#include "components/Latch.h"
#include "components/Taper.h"
#include "curve-sequencer/SequenceMode.h"
#include "curve-sequencer/StageMode.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::GenerateStage;
using dhe::curve_sequencer::SequenceMode;
using dhe::curve_sequencer::StageMode;

static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr stableHighGate = Latch{true, false};
static auto constexpr stableLowGate = Latch{false, false};

using ::testing::An;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class MockControls {
public:
  MOCK_METHOD(float, curvature, (int), (const));
  MOCK_METHOD(float, duration, (int), (const));
  MOCK_METHOD(StageMode, generateMode, (int), (const));
  MOCK_METHOD(float, input, (), (const));
  MOCK_METHOD(float, level, (int), (const));
  MOCK_METHOD(void, output, (float) );
  MOCK_METHOD(float, output, (), (const));
  MOCK_METHOD(void, showGenerating, (int, bool) );
  MOCK_METHOD(dhe::taper::VariableTaper const *, taper, (int), (const));
};

struct GenerateStageTest : public Test {
  NiceMock<MockControls> controls{};
  dhe::OneShotPhaseAccumulator phase{};

  GenerateStage<MockControls> generateStage{controls, phase};

  void givenGenerateMode(int step, StageMode mode) {
    ON_CALL(controls, generateMode(step)).WillByDefault(Return(mode));
  }

  void SetUp() override { ON_CALL(controls, taper(An<int>())).WillByDefault(Return(dhe::taper::variableTapers[0])); }
};

TEST_F(GenerateStageTest, enter_lightsStepGeneratingLight) {
  auto const step = 4;

  EXPECT_CALL(controls, showGenerating(step, true));

  generateStage.enter(step);
}

TEST_F(GenerateStageTest, exit_dimsStepGeneratingLight) {
  auto const step = 5;

  generateStage.enter(step);

  EXPECT_CALL(controls, showGenerating(step, false));

  generateStage.exit();
}

TEST_F(GenerateStageTest, highMode_returnsGenerating_ifGateIsHigh) {
  auto const step = 7;
  givenGenerateMode(step, StageMode::High);

  generateStage.enter(step);

  auto next = generateStage.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);

  next = generateStage.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);
}

TEST_F(GenerateStageTest, highMode_returnsSustaining_ifGateIsLow) {
  auto const step = 6;
  givenGenerateMode(step, StageMode::High);

  generateStage.enter(step);

  auto next = generateStage.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generateStage.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}

TEST_F(GenerateStageTest, lowMode_returnsGenerating_ifGateIsLow) {
  auto const step = 1;
  givenGenerateMode(step, StageMode::Low);

  generateStage.enter(step);

  auto next = generateStage.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);

  next = generateStage.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);
}

TEST_F(GenerateStageTest, lowMode_returnsSustaining_ifGateIsHigh) {
  auto const step = 0;
  givenGenerateMode(step, StageMode::Low);

  generateStage.enter(step);

  auto next = generateStage.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generateStage.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}

TEST_F(GenerateStageTest, calmMode_returnsGenerating_ifGateIsStable) {
  auto const step = 5;
  givenGenerateMode(step, StageMode::Calm);

  generateStage.enter(step);

  auto next = generateStage.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);

  next = generateStage.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Generating);
}

TEST_F(GenerateStageTest, calmMode_returnsSustaining_ifGateChanges) {
  auto const step = 4;
  givenGenerateMode(step, StageMode::Calm);

  generateStage.enter(step);

  auto next = generateStage.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generateStage.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}

TEST_F(GenerateStageTest, skipMode_returnsSustaining_forEveryGateState) {
  auto const step = 0;
  givenGenerateMode(step, StageMode::Skip);

  generateStage.enter(step);

  auto next = generateStage.execute(risenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generateStage.execute(fallenGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generateStage.execute(stableHighGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);

  next = generateStage.execute(stableLowGate, 0.F);
  EXPECT_EQ(next.mode, SequenceMode::Sustaining);
}
