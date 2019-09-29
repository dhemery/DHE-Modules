#include "modules/curve-sequencer/Step.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

struct FakeModule {
  MOCK_METHOD(void, setGenerating, (int, bool) );
};

float constexpr sampleTime = 1.F / 44000.F;

using Step = dhe::curve_sequencer::Step<FakeModule>;

struct StepTest : public ::testing::Test {
  int stepIndex = 3;
  FakeModule module;
  Step step{module, stepIndex};
};

TEST_F(StepTest, processSetsGeneratingTrue) {
  EXPECT_CALL(module, setGenerating(stepIndex, true));

  step.process(sampleTime);
}
