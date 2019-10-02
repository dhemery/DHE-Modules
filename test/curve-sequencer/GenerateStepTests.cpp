#include "modules/curve-sequencer/GenerateStep.h"
#include "modules/curve-sequencer/Step.h"
#include "modules/curve-sequencer/StepControls.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::curve_sequencer::GenerateStep;
using dhe::curve_sequencer::StepControls;
using Mode = dhe::curve_sequencer::Step::Mode;

struct MockStepControls : public StepControls {
  MOCK_METHOD(bool, isEnabled, (int), (const, override));
  MOCK_METHOD(int, generateMode, (int), (const, override));
  MOCK_METHOD(void, setGenerating, (int, bool), (override));
  MOCK_METHOD(int, sustainMode, (int), (const, override));
  MOCK_METHOD(void, setSustaining, (int, bool), (override));
};

auto constexpr sampleTime = 1.F / 44000.F;
auto constexpr stepIndex = 3;

using ::testing::NiceMock;
using ::testing::Return;

class GenerateStepTest : public ::testing::Test {
public:
  NiceMock<MockStepControls> controls;

  GenerateStep step{controls, stepIndex};

  void setMode(Mode mode) { ON_CALL(controls, generateMode(stepIndex)).WillByDefault(Return(mode)); }
};

TEST_F(GenerateStepTest, isUnvailableInSkipMode) {
  setMode(Mode::Skip);

  EXPECT_EQ(step.isAvailable(), false);
}

TEST_F(GenerateStepTest, isAvailableIfNotInSkipMode) {
  setMode(Mode::Rise);

  EXPECT_EQ(step.isAvailable(), true);
}

TEST_F(GenerateStepTest, processSetsGeneratingLight) {
  EXPECT_CALL(controls, setGenerating(stepIndex, true));

  step.process(sampleTime);
}
