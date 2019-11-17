#include "components/Latch.h"
#include "components/OneShotPhaseAccumulator.h"
#include "components/Taper.h"
#include "curve-sequencer/StepCondition.h"
#include "curve-sequencer/StepController.h"
#include "curve-sequencer/StepEvent.h"
#include "curve-sequencer/StepMode.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::StepCondition;
using dhe::curve_sequencer::StepController;
using dhe::curve_sequencer::StepEvent;
using dhe::curve_sequencer::StepMode;

static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr highGate = Latch{true, false};
static auto constexpr lowGate = Latch{false, false};

using ::testing::An;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class StepControllerTest : public Test {
  struct MockControls {
    MOCK_METHOD(dhe::curve_sequencer::StepCondition, condition, (int), (const));
    MOCK_METHOD(float, curvature, (int), (const));
    MOCK_METHOD(float, duration, (int), (const));
    MOCK_METHOD(bool, isEnabled, (int), (const));
    MOCK_METHOD(float, level, (int), (const));
    MOCK_METHOD(dhe::curve_sequencer::StepMode, mode, (int), (const));
    MOCK_METHOD(float, output, (), (const));
    MOCK_METHOD(void, output, (float) );
    MOCK_METHOD(void, showActive, (int, bool) );
    MOCK_METHOD(dhe::taper::VariableTaper const *, taper, (int), (const));
  };

protected:
  NiceMock<MockControls> controls{};
  dhe::OneShotPhaseAccumulator phase{};

  StepController<MockControls, dhe::OneShotPhaseAccumulator> stepController{controls, phase};

  void givenEnabled(int step, bool state) { ON_CALL(controls, isEnabled(step)).WillByDefault(Return(state)); }

  void givenMode(int step, StepMode mode) { ON_CALL(controls, mode(step)).WillByDefault(Return(mode)); }

  void givenCondition(int step, StepCondition condition) {
    ON_CALL(controls, condition(step)).WillByDefault(Return(condition));
  }

  void SetUp() override { ON_CALL(controls, taper(An<int>())).WillByDefault(Return(dhe::taper::variableTapers[0])); }
};

TEST_F(StepControllerTest, enter_lightsStepActivityLight) {
  auto constexpr step = 0;

  EXPECT_CALL(controls, showActive(step, true));

  stepController.enter(step);
}

TEST_F(StepControllerTest, exit_dimsStepActivityLight) {
  auto constexpr step = 1;

  stepController.enter(step);

  EXPECT_CALL(controls, showActive(step, false));

  stepController.exit();
}

TEST_F(StepControllerTest, curveMode_isAvailable_ifEnabled_andConditionIsNotSatisfied) {
  auto constexpr step = 1;
  givenMode(step, StepMode::Curve);
  givenEnabled(step, true);

  // DoneGenerating is never true for curve mode
  givenCondition(step, StepCondition::TimerExpires);
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

TEST_F(StepControllerTest, curveMode_isUnavailable_ifNotEnabled) {
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

TEST_F(StepControllerTest, curveMode_isUnavailable_ifConditionIsSatisfied) {
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

TEST_F(StepControllerTest, curveMode_returnsGenerated_ifConditionIsNotSatisfied) {
  auto const step = 7;
  givenMode(step, StepMode::Curve);
  givenCondition(step, StepCondition::GateIsHigh);

  stepController.enter(step);

  auto next = stepController.execute(lowGate, 0.F);
  EXPECT_EQ(next, StepEvent::Generated);

  next = stepController.execute(fallenGate, 0.F);
  EXPECT_EQ(next, StepEvent::Generated);
}

TEST_F(StepControllerTest, curveMode_returnsCompleted_ifConditionIsSatisfied) {
  auto const step = 6;
  givenMode(step, StepMode::Curve);
  givenCondition(step, StepCondition::GateIsHigh);

  stepController.enter(step);

  auto next = stepController.execute(highGate, 0.F);
  EXPECT_EQ(next, StepEvent::Completed);

  next = stepController.execute(risenGate, 0.F);
  EXPECT_EQ(next, StepEvent::Completed);
}

TEST_F(StepControllerTest, holdMode_isAvailable_ifEnabled_andConditionIsNotSatisfied) {
  auto constexpr step = 1;
  givenMode(step, StepMode::Hold);
  givenEnabled(step, true);

  // DoneGenerating is never true for hold mode
  givenCondition(step, StepCondition::TimerExpires);
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

TEST_F(StepControllerTest, holdMode_isUnavailable_ifNotEnabled) {
  auto constexpr step = 0;
  givenMode(step, StepMode::Hold);
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

TEST_F(StepControllerTest, holdMode_isUnavailable_ifConditionIsSatisfied) {
  auto constexpr step = 0;
  givenMode(step, StepMode::Hold);
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

TEST_F(StepControllerTest, holdMode_returnsGenerated_ifConditionIsNotSatisfied) {
  auto const step = 7;
  givenMode(step, StepMode::Hold);
  givenCondition(step, StepCondition::GateIsHigh);

  stepController.enter(step);

  auto next = stepController.execute(lowGate, 0.F);
  EXPECT_EQ(next, StepEvent::Generated);

  next = stepController.execute(fallenGate, 0.F);
  EXPECT_EQ(next, StepEvent::Generated);
}

TEST_F(StepControllerTest, holdMode_returnsCompleted_ifConditionIsSatisfied) {
  auto const step = 6;
  givenMode(step, StepMode::Hold);
  givenCondition(step, StepCondition::GateIsHigh);

  stepController.enter(step);

  auto next = stepController.execute(highGate, 0.F);
  EXPECT_EQ(next, StepEvent::Completed);

  next = stepController.execute(risenGate, 0.F);
  EXPECT_EQ(next, StepEvent::Completed);
}

TEST_F(StepControllerTest, sustainMode_isAvailable_ifEnabled_andConditionIsNotSatisfied) {
  auto constexpr step = 1;
  givenMode(step, StepMode::Sustain);
  givenEnabled(step, true);

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

TEST_F(StepControllerTest, sustainMode_isUnavailable_ifNotEnabled) {
  auto constexpr step = 0;
  givenMode(step, StepMode::Sustain);
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

TEST_F(StepControllerTest, sustainMode_isUnavailable_ifConditionIsSatisfied) {
  auto constexpr step = 0;
  givenMode(step, StepMode::Sustain);
  givenEnabled(step, true);

  // DoneGenerating is always true for sustain mode
  givenCondition(step, StepCondition::TimerExpires);
  EXPECT_FALSE(stepController.isAvailable(step, highGate));
  EXPECT_FALSE(stepController.isAvailable(step, lowGate));
  EXPECT_FALSE(stepController.isAvailable(step, risenGate));
  EXPECT_FALSE(stepController.isAvailable(step, fallenGate));

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

TEST_F(StepControllerTest, sustainMode_returnsGenerated_ifConditionIsNotSatisfied) {
  auto const step = 7;
  givenMode(step, StepMode::Sustain);
  givenCondition(step, StepCondition::GateIsHigh);

  stepController.enter(step);

  auto next = stepController.execute(lowGate, 0.F);
  EXPECT_EQ(next, StepEvent::Generated);

  next = stepController.execute(fallenGate, 0.F);
  EXPECT_EQ(next, StepEvent::Generated);
}

TEST_F(StepControllerTest, sustainMode_returnsCompleted_ifConditionIsSatisfied) {
  auto const step = 6;
  givenMode(step, StepMode::Sustain);
  givenCondition(step, StepCondition::GateIsHigh);

  stepController.enter(step);

  auto next = stepController.execute(highGate, 0.F);
  EXPECT_EQ(next, StepEvent::Completed);

  next = stepController.execute(risenGate, 0.F);
  EXPECT_EQ(next, StepEvent::Completed);
}
