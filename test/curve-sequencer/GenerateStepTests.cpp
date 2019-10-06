#include "modules/curve-sequencer/GenerateStep.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mocks/MockLatch.h>
#include <mocks/MockStepControls.h>

using dhe::curve_sequencer::GenerateStep;
using dhe::curve_sequencer::Step;

auto constexpr sampleTime = 1.F / 44000.F;
auto constexpr stepIndex = 3;

using ::testing::NiceMock;
using ::testing::Return;

class GenerateStepTest : public ::testing::Test {
public:
  NiceMock<MockStepControls> controls;

  GenerateStep step{controls, stepIndex};
  NiceMock<MockLatch> gateLatch;

  void setMode(Step::Mode mode) {
    auto const modeIndex = static_cast<int>(mode);
    ON_CALL(controls, generateMode(stepIndex)).WillByDefault(Return(modeIndex));
  }
};

TEST_F(GenerateStepTest, isUnvailableInSkipMode) {
  setMode(Step::Mode::Skip);

  EXPECT_EQ(step.isAvailable(), false);
}

TEST_F(GenerateStepTest, isAvailableIfNotInSkipMode) {
  setMode(Step::Mode::Rise);

  EXPECT_EQ(step.isAvailable(), true);
}

TEST_F(GenerateStepTest, processSetsGeneratingLight) {
  EXPECT_CALL(controls, setGenerating(stepIndex, true));

  step.process(gateLatch, sampleTime);
}
