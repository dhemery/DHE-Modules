#include "modules/components/Latch.h"
#include "modules/curve-sequencer/ComboStep.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mocks/MockStep.h>
#include <mocks/MockStepControls.h>

using dhe::Latch;
using dhe::curve_sequencer::ComboStep;
using ::testing::A;

auto constexpr sampleTime = 1.F / 44000.F;
auto constexpr stepIndex = 3;

using ::testing::NiceMock;
using ::testing::Return;

struct ComboStepTest : public ::testing::Test {
  Latch gateLatch;
  NiceMock<MockStepControls> controls;
  MockStep *generateStep = new NiceMock<MockStep>;
  MockStep *sustainStep = new NiceMock<MockStep>;

  ComboStep step{controls, stepIndex, generateStep, sustainStep};

  void setEnabled(bool enabled) { ON_CALL(controls, isEnabled(stepIndex)).WillByDefault(Return(enabled)); }

  void setAvailableSteps(bool generateAvailable, bool sustainAvailable) {
    ON_CALL(*generateStep, isAvailable()).WillByDefault(Return(generateAvailable));
    ON_CALL(*sustainStep, isAvailable()).WillByDefault(Return(sustainAvailable));
  }
};

struct DisabledComboStep : public ComboStepTest {
  void SetUp() override {
    ComboStepTest::SetUp();
    setEnabled(false);
  }
};

TEST_F(DisabledComboStep, isUnavailable) { EXPECT_EQ(step.isAvailable(), false); }

struct EnabledComboStep : public ComboStepTest {
  void SetUp() override {
    ComboStepTest::SetUp();
    setEnabled(true);
  }
};

TEST_F(EnabledComboStep, isAvailableIfGenerateSubstepIsAvailable) {
  setAvailableSteps(true, false);

  EXPECT_EQ(step.isAvailable(), true);
}

TEST_F(EnabledComboStep, isAvailableIfSustainSubstepIsAvailable) {
  setAvailableSteps(false, true);

  EXPECT_EQ(step.isAvailable(), true);
}

TEST_F(EnabledComboStep, isUnavailableIfBothSubstepsAreUnavailable) {
  setAvailableSteps(false, false);

  EXPECT_EQ(step.isAvailable(), false);
}

struct EnabledInactiveComboStep : public EnabledComboStep {};

struct InactiveComboStepAvailableToGenerate : public EnabledInactiveComboStep {
  void SetUp() override {
    EnabledInactiveComboStep::SetUp();
    setAvailableSteps(true, false);
  }
};

TEST_F(InactiveComboStepAvailableToGenerate, process_processesGenerateStep) {
  EXPECT_CALL(*generateStep, process(A<Latch const &>(), sampleTime));

  step.process(gateLatch, sampleTime);
}

struct InactiveComboStepAvailableToSustain : public EnabledInactiveComboStep {
  void SetUp() override {
    EnabledInactiveComboStep::SetUp();
    setAvailableSteps(false, true);
  }
};

TEST_F(InactiveComboStepAvailableToSustain, process_processesSustainStep) {
  EXPECT_CALL(*sustainStep, process(A<Latch const &>(), sampleTime));

  step.process(gateLatch, sampleTime);
}
