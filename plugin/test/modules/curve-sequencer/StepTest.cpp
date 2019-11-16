#include "curve-sequencer/Step.h"

#include "components/Latch.h"
#include "components/OneShotPhaseAccumulator.h"
#include "components/Taper.h"
#include "curve-sequencer/SequenceMode.h"
#include "curve-sequencer/StepCondition.h"
#include "curve-sequencer/StepMode.h"
#include "mocks.h"

#include <gmock/gmock-actions.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::SequenceMode;
using dhe::curve_sequencer::Step;
using dhe::curve_sequencer::StepCondition;
using dhe::curve_sequencer::StepMode;

static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr highGate = Latch{true, false};
static auto constexpr lowGate = Latch{false, false};

using ::testing::An;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

struct StepTest : public Test {
  NiceMock<MockControls> controls{};
  dhe::OneShotPhaseAccumulator phase{};

  Step<MockControls, dhe::OneShotPhaseAccumulator> stepController{controls, phase};

  void givenEnabled(int step, bool state) { ON_CALL(controls, isEnabled(step)).WillByDefault(Return(state)); }

  void givenMode(int step, StepMode mode) { ON_CALL(controls, mode(step)).WillByDefault(Return(mode)); }

  void givenCondition(int step, StepCondition condition) {
    ON_CALL(controls, condition(step)).WillByDefault(Return(condition));
  }

  void SetUp() override { ON_CALL(controls, taper(An<int>())).WillByDefault(Return(dhe::taper::variableTapers[0])); }
};

TEST_F(StepTest, enter_lightsStepActivityLight) {
  auto constexpr step = 0;

  EXPECT_CALL(controls, showActive(step, true));

  stepController.enter(step);
}

TEST_F(StepTest, exit_dimsStepActivityLight) {
  auto constexpr step = 1;

  stepController.enter(step);

  EXPECT_CALL(controls, showActive(step, false));

  stepController.exit();
}

TEST_F(StepTest, curveMode_isAvailable_ifEnabled_andConditionIsNotSatisfied) {
  auto constexpr step = 1;
  givenMode(step, StepMode::Curve);
  givenEnabled(step, true);

  givenCondition(step, StepCondition::DoneGenerating);
  EXPECT_TRUE(stepController.isAvailable(step, highGate));
  EXPECT_TRUE(stepController.isAvailable(step, lowGate));
  EXPECT_TRUE(stepController.isAvailable(step, risenGate));
  EXPECT_TRUE(stepController.isAvailable(step, fallenGate));

  givenCondition(step, StepCondition::GateIsHigh);
  EXPECT_TRUE(stepController.isAvailable(step, lowGate));
  EXPECT_TRUE(stepController.isAvailable(step, fallenGate));

  givenCondition(step, StepCondition::GateIsLow);
  EXPECT_TRUE(stepController.isAvailable(step, highGate));
  EXPECT_TRUE(stepController.isAvailable(step, risenGate));

  givenCondition(step, StepCondition::GateRises);
  EXPECT_TRUE(stepController.isAvailable(step, highGate));
  EXPECT_TRUE(stepController.isAvailable(step, lowGate));
  EXPECT_TRUE(stepController.isAvailable(step, fallenGate));

  givenCondition(step, StepCondition::GateFalls);
  EXPECT_TRUE(stepController.isAvailable(step, highGate));
  EXPECT_TRUE(stepController.isAvailable(step, lowGate));
  EXPECT_TRUE(stepController.isAvailable(step, risenGate));

  givenCondition(step, StepCondition::GateChanges);
  EXPECT_TRUE(stepController.isAvailable(step, lowGate));
  EXPECT_TRUE(stepController.isAvailable(step, highGate));

}

TEST_F(StepTest, curveMode_isUnavailable_ifNotEnabled) {
  auto constexpr step = 0;
  givenMode(step, StepMode::Curve);
  givenEnabled(step, false);

  givenCondition(step, StepCondition::GateIsHigh);
  EXPECT_FALSE(stepController.isAvailable(step, highGate));

  givenCondition(step, StepCondition::GateIsLow);
  EXPECT_FALSE(stepController.isAvailable(step, lowGate));

  givenCondition(step, StepCondition::GateRises);
  EXPECT_FALSE(stepController.isAvailable(step, risenGate));

  givenCondition(step, StepCondition::GateFalls);
  EXPECT_FALSE(stepController.isAvailable(step, fallenGate));

  givenCondition(step, StepCondition::GateChanges);
  EXPECT_FALSE(stepController.isAvailable(step, risenGate));
  EXPECT_FALSE(stepController.isAvailable(step, fallenGate));
}

TEST_F(StepTest, curveMode_isUnavailable_ifConditionIsSatisfied) {
  auto constexpr step = 0;
  givenMode(step, StepMode::Curve);
  givenEnabled(step, true);

  givenCondition(step, StepCondition::GateIsHigh);
  EXPECT_FALSE(stepController.isAvailable(step, risenGate));
  EXPECT_FALSE(stepController.isAvailable(step, highGate));

  givenCondition(step, StepCondition::GateIsLow);
  EXPECT_FALSE(stepController.isAvailable(step, fallenGate));
  EXPECT_FALSE(stepController.isAvailable(step, lowGate));

  givenCondition(step, StepCondition::GateRises);
  EXPECT_FALSE(stepController.isAvailable(step, risenGate));

  givenCondition(step, StepCondition::GateFalls);
  EXPECT_FALSE(stepController.isAvailable(step, fallenGate));

  givenCondition(step, StepCondition::GateChanges);
  EXPECT_FALSE(stepController.isAvailable(step, risenGate));
  EXPECT_FALSE(stepController.isAvailable(step, fallenGate));
}


TEST_F(StepTest, curveMode_returnsGenerating_ifConditionIsNotSatisfied) {
  auto const step = 7;
  givenCondition(step, StepCondition::GateIsHigh);

  stepController.enter(step);

  auto next = stepController.execute(risenGate, 0.F);
  EXPECT_EQ(next, SequenceMode::Generating);

  next = stepController.execute(highGate, 0.F);
  EXPECT_EQ(next, SequenceMode::Generating);
}

TEST_F(StepTest, curveMode_returnsAdvancing_ifConditionIsNotSatisfied) {
  auto const step = 6;
  givenCondition(step, StepCondition::GateIsHigh);

  stepController.enter(step);

  auto next = stepController.execute(fallenGate, 0.F);
  EXPECT_EQ(next, SequenceMode::Advancing);

  next = stepController.execute(lowGate, 0.F);
  EXPECT_EQ(next, SequenceMode::Advancing);
}

