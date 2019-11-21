#include "curve-sequencer/CurveSequencer.h"

#include "components/Latch.h"
#include "curve-sequencer/StepEvent.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::CurveSequencer;
using dhe::curve_sequencer::StepEvent;
using ::testing::A;
using ::testing::An;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;

class CurveSequencerTest : public ::testing::Test {
  struct MockControls {
    MOCK_METHOD(float, input, (), (const));
    MOCK_METHOD(bool, isGated, (), (const));
    MOCK_METHOD(bool, isLooping, (), (const));
    MOCK_METHOD(bool, isReset, (), (const));
    MOCK_METHOD(bool, isRunning, (), (const));
    MOCK_METHOD(void, output, (float) );
  };

  struct MockStepSelector {
    MOCK_METHOD(int, first, (), (const));
    MOCK_METHOD(int, successor, (int, bool), (const));
  };

  struct MockStepController {
    MOCK_METHOD(void, enter, (int) );
    MOCK_METHOD(StepEvent, execute, (Latch const &, float) );
    MOCK_METHOD(void, exit, ());
  };

protected:
  NiceMock<MockControls> controls{};
  NiceMock<MockStepSelector> stepSelector{};
  NiceMock<MockStepController> stepController{};
  CurveSequencer<MockControls, MockStepSelector, MockStepController> curveSequencer{controls, stepSelector,
                                                                                    stepController};

  void givenGate(bool state) { ON_CALL(controls, isGated()).WillByDefault(Return(state)); }
  void givenInput(float input) { ON_CALL(controls, input()).WillByDefault(Return(input)); }
  void givenReset(bool state) { ON_CALL(controls, isReset()).WillByDefault(Return(state)); }
  void givenRun(bool state) { ON_CALL(controls, isRunning()).WillByDefault(Return(state)); }

  void givenPaused() {
    givenRun(false);
    curveSequencer.execute(0.F);
  }

  void givenIdle() {
    givenRun(false);
    givenReset(true);
    givenGate(false);
    curveSequencer.execute(0.F);

    givenRun(true);
    givenReset(false);
    givenGate(false);
    curveSequencer.execute(0.F);
  }

  void expectNoControlsCommands() { EXPECT_CALL(controls, output(A<float>())).Times(0); }

  void expectNoStepCommands() {
    EXPECT_CALL(stepController, enter(An<int>())).Times(0);
    EXPECT_CALL(stepController, execute(A<Latch const &>(), A<float>())).Times(0);
    EXPECT_CALL(stepController, exit()).Times(0);
  }

  void expectNoCommands() {
    expectNoControlsCommands();
    expectNoStepCommands();
  }
};

TEST_F(CurveSequencerTest, whilePaused_runLow_emitsNothing) {
  givenPaused();
  givenRun(false);

  expectNoCommands();

  curveSequencer.execute(0.F);
}

TEST_F(CurveSequencerTest, whileIdle_gateLow_remainsIdle) {
  givenIdle();
  givenGate(false);

  // TODO: How to detect idle more definitively?
  expectNoCommands();

  curveSequencer.execute(0.F);
}

TEST_F(CurveSequencerTest,
       whileIdleWithAvailableSteps_gateRise_executesFirstAvailableStepWithEdgelessGateLatchAndSampleTime) {
  givenIdle();
  givenGate(true);

  auto constexpr step{3};
  auto constexpr sampleTime{0.39947};

  ON_CALL(stepSelector, first()).WillByDefault(Return(step));

  auto constexpr edgelessHighGateLatch = Latch{true, false};
  EXPECT_CALL(stepController, enter(step));
  EXPECT_CALL(stepController, execute(edgelessHighGateLatch, sampleTime)).WillOnce(Return(StepEvent::Generated));

  curveSequencer.execute(sampleTime);
}

TEST_F(CurveSequencerTest, whileIdleWithNoAvailableStep_gateRise_changesNothing) {
  givenIdle();
  givenGate(true);

  ON_CALL(stepSelector, first()).WillByDefault(Return(-1));

  expectNoCommands();

  curveSequencer.execute(0.F);
}

TEST_F(CurveSequencerTest, whilePaused_resetHigh_doesNotEmitOutput) {
  givenPaused();
  givenReset(true);

  givenInput(0.63483F);

  EXPECT_CALL(controls, output(A<float>())).Times(0);

  curveSequencer.execute(0.F);
}

TEST_F(CurveSequencerTest, whilePaused_resetLow_doesNotEmitOutput) {
  givenPaused();
  givenReset(false);

  givenInput(0.7867233F);

  EXPECT_CALL(controls, output(A<float>())).Times(0);

  curveSequencer.execute(0.F);
}

TEST_F(CurveSequencerTest, whileIdle_resetHigh_copiesInputVoltageToOutput) {
  givenIdle();
  givenReset(true);

  auto constexpr input{0.12938F};
  givenInput(input);

  EXPECT_CALL(controls, output(input));

  curveSequencer.execute(0.F);
}

TEST_F(CurveSequencerTest, whileIdle_resetLow_doesNotEmitOutput) {
  givenIdle();
  givenReset(false);

  givenInput(0.934F);

  EXPECT_CALL(controls, output(A<float>())).Times(0);

  curveSequencer.execute(0.F);
}
