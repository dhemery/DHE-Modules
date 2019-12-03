#include "modules/curve-sequencer/StepController.h"

#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "components/Taper.h"
#include "modules/curve-sequencer/AdvanceCondition.h"
#include "modules/curve-sequencer/StepEvent.h"
#include "modules/curve-sequencer/StepMode.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::PhaseTimer;
using dhe::curve_sequencer::AdvanceCondition;
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
    MOCK_METHOD(dhe::curve_sequencer::AdvanceCondition, condition, (int), (const));
    MOCK_METHOD(float, curvature, (int), (const));
    MOCK_METHOD(float, duration, (int), (const));
    MOCK_METHOD(float, level, (int), (const));
    MOCK_METHOD(dhe::curve_sequencer::StepMode, mode, (int), (const));
    MOCK_METHOD(float, output, (), (const));
    MOCK_METHOD(void, output, (float) );
    MOCK_METHOD(void, showInactive, (int) );
    MOCK_METHOD(void, showProgress, (int, float) );
    MOCK_METHOD(dhe::taper::VariableTaper const *, taper, (int), (const));
  };

protected:
  NiceMock<MockControls> controls{};
  PhaseTimer timer{};

  StepController<MockControls> stepController{controls, timer};

  void givenMode(int step, StepMode mode) { ON_CALL(controls, mode(step)).WillByDefault(Return(mode)); }

  void givenCondition(int step, AdvanceCondition condition) {
    ON_CALL(controls, condition(step)).WillByDefault(Return(condition));
  }

  void SetUp() override {
    ON_CALL(controls, duration(An<int>())).WillByDefault(Return(1.F));
    ON_CALL(controls, taper(An<int>())).WillByDefault(Return(dhe::taper::variableTapers[0]));
  }
};

TEST_F(StepControllerTest, enter_showsStepIsAt0Progress) {
  auto constexpr step = 0;

  EXPECT_CALL(controls, showProgress(step, 0.F));

  stepController.enter(step);
}

TEST_F(StepControllerTest, exit_showsStepInactive) {
  auto constexpr step = 1;

  stepController.enter(step);

  EXPECT_CALL(controls, showInactive(step));

  stepController.exit();
}

// TODO: advance on time returns generated if timer does not expire
// TODO: advance on time returns completed if timer expires

TEST_F(StepControllerTest, advanceOnGateRise_returnsCompleted_ifGateRises) {
  auto const step = 2;
  givenCondition(step, AdvanceCondition::GateRises);

  stepController.enter(step);

  givenMode(step, StepMode::Curve);
  EXPECT_EQ(stepController.execute(risenGate, 0.F), StepEvent::Completed);

  givenMode(step, StepMode::Hold);
  EXPECT_EQ(stepController.execute(risenGate, 0.F), StepEvent::Completed);

  givenMode(step, StepMode::Sustain);
  EXPECT_EQ(stepController.execute(risenGate, 0.F), StepEvent::Completed);
}

TEST_F(StepControllerTest, advanceOnGateRise_returnsGenerated_ifGateDoesNotRise) {
  auto const step = 3;
  givenCondition(step, AdvanceCondition::GateRises);

  stepController.enter(step);

  givenMode(step, StepMode::Curve);
  EXPECT_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Generated);

  givenMode(step, StepMode::Hold);
  EXPECT_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Generated);

  givenMode(step, StepMode::Sustain);
  EXPECT_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Generated);
}

TEST_F(StepControllerTest, advanceOnGateFall_returnsCompleted_ifGateFalls) {
  auto const step = 2;
  givenCondition(step, AdvanceCondition::GateFalls);

  stepController.enter(step);

  givenMode(step, StepMode::Curve);
  EXPECT_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Completed);

  givenMode(step, StepMode::Hold);
  EXPECT_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Completed);

  givenMode(step, StepMode::Sustain);
  EXPECT_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Completed);
}

TEST_F(StepControllerTest, advanceOnGateFall_returnsGenerated_ifGateDoesNotFall) {
  auto const step = 3;
  givenCondition(step, AdvanceCondition::GateFalls);

  stepController.enter(step);

  givenMode(step, StepMode::Curve);
  EXPECT_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(risenGate, 0.F), StepEvent::Generated);

  givenMode(step, StepMode::Hold);
  EXPECT_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(risenGate, 0.F), StepEvent::Generated);

  givenMode(step, StepMode::Sustain);
  EXPECT_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(risenGate, 0.F), StepEvent::Generated);
}

TEST_F(StepControllerTest, advanceOnGateChange_returnsCompleted_ifGateChanges) {
  auto const step = 4;
  givenCondition(step, AdvanceCondition::GateChanges);

  stepController.enter(step);

  givenMode(step, StepMode::Curve);
  EXPECT_EQ(stepController.execute(risenGate, 0.F), StepEvent::Completed);
  EXPECT_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Completed);

  givenMode(step, StepMode::Hold);
  EXPECT_EQ(stepController.execute(risenGate, 0.F), StepEvent::Completed);
  EXPECT_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Completed);

  givenMode(step, StepMode::Sustain);
  EXPECT_EQ(stepController.execute(risenGate, 0.F), StepEvent::Completed);
  EXPECT_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Completed);
}

TEST_F(StepControllerTest, advanceOnGateChange_returnsGenerated_ifGateDoesNotChange) {
  auto const step = 5;
  givenCondition(step, AdvanceCondition::GateChanges);

  stepController.enter(step);

  givenMode(step, StepMode::Curve);
  EXPECT_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);

  givenMode(step, StepMode::Hold);
  EXPECT_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);

  givenMode(step, StepMode::Sustain);
  EXPECT_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);
}

TEST_F(StepControllerTest, advanceOnGateHigh_returnsGenerated_ifGateIsLow) {
  auto const step = 6;
  givenCondition(step, AdvanceCondition::GateIsHigh);

  stepController.enter(step);

  givenMode(step, StepMode::Curve);
  EXPECT_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Generated);

  givenMode(step, StepMode::Hold);
  EXPECT_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Generated);

  givenMode(step, StepMode::Sustain);
  EXPECT_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Generated);
}

TEST_F(StepControllerTest, advanceOnGateHigh_returnsCompleted_ifGateIsHigh) {
  auto const step = 7;
  givenCondition(step, AdvanceCondition::GateIsHigh);

  stepController.enter(step);

  givenMode(step, StepMode::Curve);
  EXPECT_EQ(stepController.execute(highGate, 0.F), StepEvent::Completed);
  EXPECT_EQ(stepController.execute(risenGate, 0.F), StepEvent::Completed);

  givenMode(step, StepMode::Hold);
  EXPECT_EQ(stepController.execute(highGate, 0.F), StepEvent::Completed);
  EXPECT_EQ(stepController.execute(risenGate, 0.F), StepEvent::Completed);

  givenMode(step, StepMode::Sustain);
  EXPECT_EQ(stepController.execute(highGate, 0.F), StepEvent::Completed);
  EXPECT_EQ(stepController.execute(risenGate, 0.F), StepEvent::Completed);
}

TEST_F(StepControllerTest, advanceOnGateLow_returnsGenerated_ifGateIsHigh) {
  auto const step = 0;
  givenCondition(step, AdvanceCondition::GateIsLow);

  stepController.enter(step);

  givenMode(step, StepMode::Curve);
  EXPECT_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(risenGate, 0.F), StepEvent::Generated);

  givenMode(step, StepMode::Hold);
  EXPECT_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(risenGate, 0.F), StepEvent::Generated);

  givenMode(step, StepMode::Sustain);
  EXPECT_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
  EXPECT_EQ(stepController.execute(risenGate, 0.F), StepEvent::Generated);
}

TEST_F(StepControllerTest, advanceOnGateLow_returnsCompleted_ifGateIsLow) {
  auto const step = 1;
  givenCondition(step, AdvanceCondition::GateIsLow);

  stepController.enter(step);

  givenMode(step, StepMode::Curve);
  EXPECT_EQ(stepController.execute(lowGate, 0.F), StepEvent::Completed);
  EXPECT_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Completed);

  givenMode(step, StepMode::Hold);
  EXPECT_EQ(stepController.execute(lowGate, 0.F), StepEvent::Completed);
  EXPECT_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Completed);

  givenMode(step, StepMode::Sustain);
  EXPECT_EQ(stepController.execute(lowGate, 0.F), StepEvent::Completed);
  EXPECT_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Completed);
}
