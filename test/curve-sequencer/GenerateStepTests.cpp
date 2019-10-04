#include "modules/curve-sequencer/GenerateStep.h"
#include "modules/curve-sequencer/StepMode.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mocks/MockLatch.h>
#include <mocks/MockStepControls.h>

using dhe::curve_sequencer::GenerateStep;
using StepMode = dhe::curve_sequencer::StepMode;

auto constexpr sampleTime = 1.F / 44000.F;
auto constexpr stepIndex = 3;

using ::testing::NiceMock;
using ::testing::Return;

class GenerateStepTest : public ::testing::Test {
public:
  NiceMock<MockStepControls> controls;

  GenerateStep step{controls, stepIndex};
  NiceMock<MockLatch> gateLatch;

  void setMode(StepMode mode) { ON_CALL(controls, generateMode(stepIndex)).WillByDefault(Return(mode)); }
};

TEST_F(GenerateStepTest, isUnvailableInSkipMode) {
  setMode(StepMode::Skip);

  EXPECT_EQ(step.isAvailable(), false);
}

TEST_F(GenerateStepTest, isAvailableIfNotInSkipMode) {
  setMode(StepMode::Rise);

  EXPECT_EQ(step.isAvailable(), true);
}

TEST_F(GenerateStepTest, processSetsGeneratingLight) {
  EXPECT_CALL(controls, setGenerating(stepIndex, true));

  step.process(gateLatch, sampleTime);
}
