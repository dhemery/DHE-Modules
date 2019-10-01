#include "modules/curve-sequencer/ModuleStep.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::curve_sequencer::ModuleStep;

struct FakeModule {
  MOCK_METHOD(bool, isEnabled, (int), (const));
  MOCK_METHOD(int, generateMode, (int) );
  MOCK_METHOD(void, setGenerating, (int, bool) );
  MOCK_METHOD(int, sustainMode, (int) );
  MOCK_METHOD(void, setSustaining, (int, bool) );
};

float constexpr sampleTime = 1.F / 44000.F;

using ::testing::NiceMock;
using ::testing::Return;

class ModuleStepTest : public ::testing::Test {
public:
  int stepIndex = 3;
  NiceMock<FakeModule> module;
  ModuleStep<FakeModule> step{&module, stepIndex};

  using Mode = ModuleStep<FakeModule>::Mode;
  void setModes(Mode generateMode, Mode sustainMode) {
    ON_CALL(module, generateMode(stepIndex)).WillByDefault(Return(generateMode));
    ON_CALL(module, sustainMode(stepIndex)).WillByDefault(Return(sustainMode));
  }
};

class InactiveModuleStep : public ModuleStepTest {};

TEST_F(InactiveModuleStep, isAvailableIfEnabledInModule) {
  ON_CALL(module, isEnabled(stepIndex)).WillByDefault(Return(true));

  EXPECT_EQ(step.isAvailable(), true);

  ON_CALL(module, isEnabled(stepIndex)).WillByDefault(Return(false));

  EXPECT_EQ(step.isAvailable(), false);
}

class InactiveModuleStepAvailableToGenerate : public InactiveModuleStep {
public:
  void SetUp() override {
    ON_CALL(module, generateMode(stepIndex)).WillByDefault(Return(ModuleStep<FakeModule>::Mode::Duration));
  }
};

TEST_F(InactiveModuleStepAvailableToGenerate, setsGeneratingTrueWhenProcessed) {
  EXPECT_CALL(module, setGenerating(stepIndex, true));

  step.process(sampleTime);
}

class InactiveModuleStepAvailableToSustain : public InactiveModuleStep {
public:
  void SetUp() override { setModes(Mode::Skip, Mode::Fall); }
};

TEST_F(InactiveModuleStepAvailableToSustain, setsSustainingTrueWhenProcessed) {
  EXPECT_CALL(module, setSustaining(stepIndex, true));

  step.process(sampleTime);
}
