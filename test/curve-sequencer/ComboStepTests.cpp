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

class ComboStepTest : public ::testing::Test {
protected:
  void givenEnabled(bool enabled) { ON_CALL(controls, isEnabled(stepIndex)).WillByDefault(Return(enabled)); }

  void givenAvailableSteps(bool generateAvailable, bool sustainAvailable) {
    ON_CALL(*generateStep, isAvailable()).WillByDefault(Return(generateAvailable));
    ON_CALL(*sustainStep, isAvailable()).WillByDefault(Return(sustainAvailable));
  }

  NiceMock<MockStepControls> controls;
  MockStep *generateStep = new NiceMock<MockStep>;
  MockStep *sustainStep = new NiceMock<MockStep>;

  ComboStep step{controls, stepIndex, generateStep, sustainStep};

};

class DisabledComboStep : public ComboStepTest {
  void SetUp() override {
    ComboStepTest::SetUp();
    givenEnabled(false);
  }
};

TEST_F(DisabledComboStep, isUnavailable) { EXPECT_EQ(step.isAvailable(), false); }

class EnabledComboStep : public ComboStepTest {
protected:
  void SetUp() override {
    ComboStepTest::SetUp();
    givenEnabled(true);
  }
};

TEST_F(EnabledComboStep, isAvailableIfGenerateSubstepIsAvailable) {
  givenAvailableSteps(true, false);

  EXPECT_EQ(step.isAvailable(), true);
}

TEST_F(EnabledComboStep, isAvailableIfSustainSubstepIsAvailable) {
  givenAvailableSteps(false, true);

  EXPECT_EQ(step.isAvailable(), true);
}

TEST_F(EnabledComboStep, isUnavailableIfBothSubstepsAreUnavailable) {
  givenAvailableSteps(false, false);

  EXPECT_EQ(step.isAvailable(), false);
}

class EnabledInactiveComboStep : public EnabledComboStep {};

class InactiveComboStepAvailableToGenerate : public EnabledInactiveComboStep {
  void SetUp() override {
    EnabledInactiveComboStep::SetUp();
    givenAvailableSteps(true, false);
  }
};

TEST_F(InactiveComboStepAvailableToGenerate, process_processesGenerateStep) {
  EXPECT_CALL(*generateStep, process(A<Latch const &>(), sampleTime));

  step.process(Latch{}, sampleTime);
}

class InactiveComboStepAvailableToSustain : public EnabledInactiveComboStep {
  void SetUp() override {
    EnabledInactiveComboStep::SetUp();
    givenAvailableSteps(false, true);
  }
};

TEST_F(InactiveComboStepAvailableToSustain, process_processesSustainStep) {
  EXPECT_CALL(*sustainStep, process(A<Latch const &>(), sampleTime));

  step.process(Latch{}, sampleTime);
}
