#include "stage/GenerateMode.h"

#include "components/PhaseTimer.h"
#include "components/Taper.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class GenerateModeTest : public Test {
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
  using Result = dhe::stage::GenerateMode<Controls, dhe::PhaseTimer>::Result;
  static auto constexpr defaultDuration{1.F};
  static auto constexpr defaultTaper{&dhe::taper::variableJTaper};

  NiceMock<Controls> controls{};
  dhe::PhaseTimer timer{};

  dhe::stage::GenerateMode<Controls, dhe::PhaseTimer> mode{controls, timer};

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

TEST_F(GenerateModeTest, enter_showsStageActive) {
  EXPECT_CALL(controls, showActive(true));

  mode.enter(0.F);
}

TEST_F(GenerateModeTest, exit_showsStageInactive) {
  EXPECT_CALL(controls, showActive(false));

  mode.exit();
}

TEST_F(GenerateModeTest, execute_returnsGenerated_ifTimerNotExpired) {
  givenDuration(1.F);
  givenPhase(0.F);

  EXPECT_EQ(mode.execute(0.1F), Result::Generated);
}

TEST_F(GenerateModeTest, execute_returnsCompleted_ifTimerExpired) {
  givenDuration(1.F);
  givenPhase(0.9999F);

  EXPECT_EQ(mode.execute(0.1F), Result::Completed);
}

TEST_F(GenerateModeTest, execute_advancesPhase) {
  auto constexpr duration{3.3498F};
  auto constexpr sampleTime(0.9384F);

  givenPhase(0.F);
  givenDuration(duration);

  mode.execute(sampleTime);

  EXPECT_EQ(timer.phase(), sampleTime / duration);
}

TEST_F(GenerateModeTest, execute_outputsCurveVoltage) {
  givenPhase(0.F);

  // Configure linear curve rising from 4V to 6V over 1s.
  givenCurvature(0.F); // 0 curvature -> linear curve
  givenDuration(1.F);  // 1s ramp
  mode.enter(4.F);     // Start curve at 4V
  givenLevel(6.F);     // End curve at 6V
  // The full range of the curve is 2V = 6V - 4V.

  // Sample time is 1/10 of duration. So execute will raise the output by 0.2V (1/10 of the full curve).
  auto constexpr sampleTime = 0.1F;

  // Execute must raise output by 0.2V from 4V to 4.2V.
  EXPECT_CALL(controls, output(4.2F));

  mode.execute(sampleTime);
}
