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
using ::testing::NiceMock;
using ::testing::Return;

class CurveSequencerTest : public ::testing::Test {
protected:
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

  NiceMock<MockControls> controls{};
  NiceMock<MockStepSelector> stepSelector{};
  NiceMock<MockStepController> stepController{};
  CurveSequencer<MockControls, MockStepSelector, MockStepController> curveSequencer{controls, stepSelector,
                                                                                    stepController};

  void givenGate(bool state) { ON_CALL(controls, isGated()).WillByDefault(Return(state)); }
  void givenInput(float input) { ON_CALL(controls, input()).WillByDefault(Return(input)); }
  void givenReset(bool state) { ON_CALL(controls, isReset()).WillByDefault(Return(state)); }
  void givenRun(bool state) { ON_CALL(controls, isRunning()).WillByDefault(Return(state)); }

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

  void SetUp() override { ::testing::Test::SetUp(); }
};

// TODO: Test that a curve sequencer is initially idle

class PausedIdleCurveSequencer : public CurveSequencerTest {
  void SetUp() override {
    CurveSequencerTest::SetUp();
    givenRun(false);
  }
};

TEST_F(PausedIdleCurveSequencer, resetLow_doesNotEmitOutput) {
  givenReset(false);

  givenInput(0.7867233F);

  EXPECT_CALL(controls, output(A<float>())).Times(0);

  curveSequencer.execute(0.F);
}

TEST_F(PausedIdleCurveSequencer, resetHigh_doesNotEmitOutput) {
  givenReset(true);

  givenInput(0.63483F);

  EXPECT_CALL(controls, output(A<float>())).Times(0);

  curveSequencer.execute(0.123F);
}

TEST_F(PausedIdleCurveSequencer, gateLow_doesNothing) {
  givenGate(false);

  expectNoCommands();

  curveSequencer.execute(0.1F);
}

TEST_F(PausedIdleCurveSequencer, gateHigh_doesNothing) {
  givenGate(true);

  expectNoCommands();

  curveSequencer.execute(0.1F);
}

class RunningIdleCurveSequencer : public CurveSequencerTest {
  void SetUp() override {
    CurveSequencerTest::SetUp();
    givenRun(true);
  }
};

TEST_F(RunningIdleCurveSequencer, resetHigh_copiesInputVoltageToOutput) {
  givenReset(true);

  auto constexpr input{0.12938F};
  givenInput(input);

  EXPECT_CALL(controls, output(input));

  curveSequencer.execute(0.F);
}

TEST_F(RunningIdleCurveSequencer, resetLow_doesNotEmitOutput) {
  givenReset(false);

  givenInput(0.934F);

  EXPECT_CALL(controls, output(A<float>())).Times(0);

  curveSequencer.execute(0.F);
}

TEST_F(RunningIdleCurveSequencer, gateLow_doesNothing) {
  givenGate(false);

  expectNoCommands();

  curveSequencer.execute(0.1F);
}

TEST_F(RunningIdleCurveSequencer, gateRise_executesFirstEnabledStep) {
  givenGate(true);

  auto constexpr firstEnabledStep{3};
  ON_CALL(stepSelector, first()).WillByDefault(Return(firstEnabledStep));

  EXPECT_CALL(stepController, enter(firstEnabledStep));

  auto constexpr sampleTime{0.39947};
  auto constexpr edgelessHighGateLatch = Latch{true, false};

  // The sequencer must clear the edge before executing the step
  EXPECT_CALL(stepController, execute(edgelessHighGateLatch, sampleTime)).WillOnce(Return(StepEvent::Generated));

  curveSequencer.execute(sampleTime);
}

TEST_F(RunningIdleCurveSequencer, gateRise_doesNothing_ifNoEnabledStep) {
  givenGate(true);

  ON_CALL(stepSelector, first()).WillByDefault(Return(-1));

  expectNoCommands();

  curveSequencer.execute(0.F);
}
