#include "modules/curve-sequencer/Step.h"
#include "modules/curve-sequencer/StepControls.h"
#include "modules/curve-sequencer/SustainStep.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::curve_sequencer::StepControls;
using dhe::curve_sequencer::SustainStep;
using StepMode = dhe::curve_sequencer::StepMode;

struct MockStepControls : public StepControls {
  MOCK_METHOD(bool, isEnabled, (int), (const, override));
  MOCK_METHOD(StepMode, generateMode, (int), (const, override));
  MOCK_METHOD(void, setGenerating, (int, bool), (override));
  MOCK_METHOD(StepMode, sustainMode, (int), (const, override));
  MOCK_METHOD(void, setSustaining, (int, bool), (override));
};

auto constexpr sampleTime = 1.F / 44000.F;
auto constexpr stepIndex = 3;

using ::testing::NiceMock;
using ::testing::Return;

class SustainStepTest : public ::testing::Test {
public:
  NiceMock<MockStepControls> controls;

  SustainStep step{controls, stepIndex};

  void setMode(StepMode mode) { ON_CALL(controls, sustainMode(stepIndex)).WillByDefault(Return(mode)); }
};

TEST_F(SustainStepTest, isUnvailableInSkipMode) {
  setMode(StepMode::Skip);

  EXPECT_EQ(step.isAvailable(), false);
}

TEST_F(SustainStepTest, isAvailableIfNotInSkipMode) {
  setMode(StepMode::Rise);

  EXPECT_EQ(step.isAvailable(), true);
}

TEST_F(SustainStepTest, processSetsSustainingLight) {
  EXPECT_CALL(controls, setSustaining(stepIndex, true));

  step.process(sampleTime);
}
