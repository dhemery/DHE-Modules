#include "modules/curve-sequencer/ModuleStep.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::curve_sequencer::ModuleStep;

struct FakeModule {
  MOCK_METHOD(void, setGenerating, (int, bool) );
  MOCK_METHOD(bool, isEnabled, (int), (const));
};

float constexpr sampleTime = 1.F / 44000.F;

using ::testing::NiceMock;
using ::testing::Return;

struct ModuleStepTest : public ::testing::Test {
  int stepIndex = 3;
  NiceMock<FakeModule> module;
  ModuleStep<FakeModule> step{&module, stepIndex};
};

TEST_F(ModuleStepTest, processSetsGeneratingTrue) {
  EXPECT_CALL(module, setGenerating(stepIndex, true));

  step.process(sampleTime);
}

TEST_F(ModuleStepTest, isAvailableIfEnabledInModule) {
  ON_CALL(module, isEnabled(stepIndex)).WillByDefault(Return(true));

  EXPECT_EQ(step.isAvailable(), true);

  ON_CALL(module, isEnabled(stepIndex)).WillByDefault(Return(false));

  EXPECT_EQ(step.isAvailable(), false);
}