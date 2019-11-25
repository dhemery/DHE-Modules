#include "stage/HoldMode.h"

#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "components/Taper.h"
#include "stage/Event.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::PhaseTimer;
using dhe::stage::Event;
using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class HoldModeTest : public Test {
  class Controls {
  public:
    MOCK_METHOD(float, curvature, (), (const));
    MOCK_METHOD(float, input, (), (const));
    MOCK_METHOD(float, duration, (), (const));
    MOCK_METHOD(float, level, (), (const));
    MOCK_METHOD(void, showActive, (bool), ());
    MOCK_METHOD(void, showEoc, (bool), ());
    MOCK_METHOD(void, output, (float), ());
    MOCK_METHOD(dhe::taper::VariableTaper const *, taper, (), (const));
  };

protected:
  static auto constexpr defaultDuration{1.F};
  static auto constexpr defaultTaper{&dhe::taper::variableJTaper};

  NiceMock<Controls> controls{};
  PhaseTimer timer{};

  dhe::stage::HoldMode<Controls> holdMode{controls, timer};

  void givenCurvature(float curvature) { ON_CALL(controls, curvature()).WillByDefault(Return(curvature)); }
  void givenDuration(float duration) { ON_CALL(controls, duration()).WillByDefault(Return(duration)); }
  void givenInput(float input) { ON_CALL(controls, input()).WillByDefault(Return(input)); }
  void givenLevel(float level) { ON_CALL(controls, level()).WillByDefault(Return(level)); }
  void givenTaper(dhe::taper::VariableTaper const *taper) { ON_CALL(controls, taper()).WillByDefault(Return(taper)); }

  void givenPhase(float phase) {
    timer.reset();
    timer.advance(phase);
  }

  void SetUp() override {
    Test::SetUp();
    givenTaper(defaultTaper);
    givenDuration(defaultDuration);
  }
};

TEST_F(HoldModeTest, enter_resetsTimer) {
  timer.advance(0.5F);

  holdMode.enter();

  EXPECT_EQ(timer.phase(), 0.F);
}

TEST_F(HoldModeTest, enter_showsStageActive) {
  EXPECT_CALL(controls, showActive(true));

  holdMode.enter();
}

TEST_F(HoldModeTest, exit_showsStageInactive) {
  EXPECT_CALL(controls, showActive(false));

  holdMode.exit();
}

TEST_F(HoldModeTest, execute_returnsGenerated_ifTimerNotExpired) {
  givenDuration(1.F);
  givenPhase(0.F);

  EXPECT_EQ(holdMode.execute(Latch{}, 0.1F), Event::Generated);
}

TEST_F(HoldModeTest, execute_returnsCompleted_ifTimerExpired) {
  givenDuration(1.F);
  givenPhase(0.9999F);

  EXPECT_EQ(holdMode.execute(Latch{}, 0.1F), Event::Completed);
}

TEST_F(HoldModeTest, execute_resetsTimer_ifRetriggerRises) {
  timer.advance(0.5F);

  holdMode.execute(Latch{true, true}, 0.F);

  EXPECT_EQ(timer.phase(), 0.F);
}

// TODO: How to test this?
TEST_F(HoldModeTest, execute_setsCurveStartingVoltageToInputVoltage_ifRetriggerRises) {
  holdMode.execute(Latch{true, true}, 0.F);
}

TEST_F(HoldModeTest, execute_advancesPhase) {
  auto constexpr duration{3.3498F};
  auto constexpr sampleTime(0.9384F);

  givenPhase(0.F);
  givenDuration(duration);

  holdMode.execute(Latch{}, sampleTime);

  EXPECT_EQ(timer.phase(), sampleTime / duration);
}

TEST_F(HoldModeTest, execute_outputsNothing) {
  EXPECT_CALL(controls, output(A<float>())).Times(0);

  holdMode.execute(Latch{}, 0.F);
}
