#include "modules/curve-sequencer/ComboStep.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::curve_sequencer::ComboStep;
using dhe::curve_sequencer::Step;
using dhe::curve_sequencer::StepControls;
using StepMode = dhe::curve_sequencer::StepMode;

struct MockStepControls : public StepControls {
  MOCK_METHOD(bool, isEnabled, (int), (const, override));
  MOCK_METHOD(StepMode, generateMode, (int), (const, override));
  MOCK_METHOD(void, setGenerating, (int, bool), (override));
  MOCK_METHOD(StepMode, sustainMode, (int), (const, override));
  MOCK_METHOD(void, setSustaining, (int, bool), (override));
};

struct MockStep : public Step {
  MOCK_METHOD(bool, isAvailable, (), (const, override));
  MOCK_METHOD(void, process, (float), (override));
};

auto constexpr sampleTime = 1.F / 44000.F;
auto constexpr stepIndex = 3;

using ::testing::NiceMock;
using ::testing::Return;

class ComboStepTest : public ::testing::Test {
public:
  NiceMock<MockStepControls> controls;
  MockStep *generateStep = new NiceMock<MockStep>;
  MockStep *sustainStep = new NiceMock<MockStep>;

  ComboStep step{controls, stepIndex, generateStep, sustainStep};

  void setEnabledByControls(bool enabled) { ON_CALL(controls, isEnabled(stepIndex)).WillByDefault(Return(enabled)); }

  void setAvailableSteps(bool generateAvailable, bool sustainAvailable) {
    ON_CALL(*generateStep, isAvailable()).WillByDefault(Return(generateAvailable));
    ON_CALL(*sustainStep, isAvailable()).WillByDefault(Return(sustainAvailable));
  }
};

class InactiveComboStep : public ComboStepTest {};

TEST_F(InactiveComboStep, isUnavailableIfDisabledByControls) {
  setEnabledByControls(false);

  EXPECT_EQ(step.isAvailable(), false);
}

class EnabledInactiveComboStep : public ComboStepTest {
public:
  void SetUp() override {
    ComboStepTest::SetUp();
    setEnabledByControls(true);
  }
};

class InactiveComboStepAvailableToGenerate : public EnabledInactiveComboStep {
public:
  void SetUp() override {
    EnabledInactiveComboStep::SetUp();
    setAvailableSteps(true, false);
  }
};

TEST_F(InactiveComboStepAvailableToGenerate, process_processesGenerateStep) {
  EXPECT_CALL(*generateStep, process(sampleTime));

  step.process(sampleTime);
}

class InactiveComboStepAvailableToSustain : public EnabledInactiveComboStep {
public:
  void SetUp() override {
    EnabledInactiveComboStep::SetUp();
    setAvailableSteps(false, true);
  }
};

TEST_F(InactiveComboStepAvailableToSustain, process_processesSustainStep) {
  EXPECT_CALL(*sustainStep, process(sampleTime));

  step.process(sampleTime);
}
