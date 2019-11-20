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
    MOCK_METHOD(dhe::curve_sequencer::StepEvent, execute, (dhe::Latch const &, float) );
    MOCK_METHOD(void, exit, ());
  };

protected:
  static auto constexpr sampleTime{1.F / 12345.F};

  NiceMock<MockControls> controls{};
  NiceMock<MockStepSelector> stepSelector{};
  NiceMock<MockStepController> stepController{};
  CurveSequencer<MockControls, MockStepSelector, MockStepController> curveSequencer{controls, stepSelector,
                                                                                    stepController};

  void givenGate(bool state) { ON_CALL(controls, isGated()).WillByDefault(Return(state)); }

  void givenRun(bool state) { ON_CALL(controls, isRunning()).WillByDefault(Return(state)); }

  void givenInitialized() {
    Mock::VerifyAndClear(&controls);
    Mock::VerifyAndClear(&stepSelector);
    Mock::VerifyAndClear(&stepController);
  }

  void givenIdling() {
    givenInitialized();
    givenRun(true);
  }
};

TEST_F(CurveSequencerTest, runLowWhilePaused_doesNothing) {
  givenRun(false);
  // TODO: Test for no interactions on step selector, generate mode, and sustain mode.
}

TEST_F(CurveSequencerTest, runRiseWhilePaused_BeginsIdling) {
  givenInitialized();
  // TODO: How to test for idling?
}

TEST_F(CurveSequencerTest, gateLowWhileIdling_doesNothing) {
  givenIdling();

  EXPECT_CALL(stepSelector, first()).Times(0);

  curveSequencer.execute(sampleTime);
}

TEST_F(CurveSequencerTest, gateRiseWhileIdling_generatesFirstAvailableStep) {
  givenIdling();
  givenGate(true);

  auto constexpr step{3};

  ON_CALL(stepSelector, first()).WillByDefault(Return(step));

  EXPECT_CALL(stepController, enter(step));
  EXPECT_CALL(stepController, execute(A<Latch const &>(), sampleTime)).WillOnce(Return(StepEvent::Generated));

  curveSequencer.execute(sampleTime);
}

// TODO: Idling + Gate rise sustains first if generating returns sustaining

// TODO: Idling + Gate rise does nothing if no available step
