#include "modules/curve-sequencer/IndexedStep.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::curve_sequencer::IndexedStep;
using dhe::curve_sequencer::StepControls;

struct FakeStepControls : public StepControls {
  MOCK_METHOD(bool, isEnabled, (int), (const, override));
  MOCK_METHOD(int, generateMode, (int), (const, override));
  MOCK_METHOD(void, setGenerating, (int, bool), (override));
  MOCK_METHOD(int, sustainMode, (int), (const, override));
  MOCK_METHOD(void, setSustaining, (int, bool), (override));
};

float constexpr sampleTime = 1.F / 44000.F;

using ::testing::NiceMock;
using ::testing::Return;

class IndexedStepTest : public ::testing::Test {
public:
  int stepIndex = 3;
  NiceMock<FakeStepControls> module;
  IndexedStep step{module, stepIndex};

  using Mode = IndexedStep::Mode;
  void setModes(Mode generateMode, Mode sustainMode) {
    ON_CALL(module, generateMode(stepIndex)).WillByDefault(Return(generateMode));
    ON_CALL(module, sustainMode(stepIndex)).WillByDefault(Return(sustainMode));
  }
};

class InactiveStep : public IndexedStepTest {};

TEST_F(InactiveStep, isAvailableIfEnabledInModule) {
  ON_CALL(module, isEnabled(stepIndex)).WillByDefault(Return(true));

  EXPECT_EQ(step.isAvailable(), true);

  ON_CALL(module, isEnabled(stepIndex)).WillByDefault(Return(false));

  EXPECT_EQ(step.isAvailable(), false);
}

class InactiveStepAvailableToGenerate : public InactiveStep {
public:
  void SetUp() override {
    ON_CALL(module, generateMode(stepIndex)).WillByDefault(Return(Mode::Duration));
  }
};

TEST_F(InactiveStepAvailableToGenerate, setsGeneratingTrueWhenProcessed) {
  EXPECT_CALL(module, setGenerating(stepIndex, true));

  step.process(sampleTime);
}

class InactiveStepAvailableToSustain : public InactiveStep {
public:
  void SetUp() override { setModes(Mode::Skip, Mode::Fall); }
};

TEST_F(InactiveStepAvailableToSustain, setsSustainingTrueWhenProcessed) {
  EXPECT_CALL(module, setSustaining(stepIndex, true));

  step.process(sampleTime);
}
