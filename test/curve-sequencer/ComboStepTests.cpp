#include "modules/components/Latch.h"
#include "modules/curve-sequencer/ComboStep.h"
#include "modules/curve-sequencer/StepMode.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::ComboStep;
using dhe::curve_sequencer::Step;
using dhe::curve_sequencer::StepControls;
using StepMode = dhe::curve_sequencer::StepMode;
using ::testing::A;

struct MockStepControls : public StepControls {
  MOCK_METHOD(bool, isEnabled, (int), (const, override));
  MOCK_METHOD(StepMode, generateMode, (int), (const, override));
  MOCK_METHOD(void, setGenerating, (int, bool), (override));
  MOCK_METHOD(StepMode, sustainMode, (int), (const, override));
  MOCK_METHOD(void, setSustaining, (int, bool), (override));
};

struct MockStep : public Step {
  MOCK_METHOD(bool, isAvailable, (), (const, override));
  MOCK_METHOD(void, process, (Latch const &, float), (override));
};

struct MockLatch : public Latch {
  MOCK_METHOD(bool, isEdge, (), (const, override));
  MOCK_METHOD(bool, isHigh, (), (const, override));
  MOCK_METHOD(void, set, (bool, bool), (override));
  MOCK_METHOD(void, step, (), (override));
};

auto constexpr sampleTime = 1.F / 44000.F;
auto constexpr stepIndex = 3;

using ::testing::NiceMock;
using ::testing::Return;

class ComboStepTest : public ::testing::Test {
public:
  NiceMock<MockStepControls> controls;
  NiceMock<MockLatch> gateLatch;
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
  EXPECT_CALL(*generateStep, process(A<Latch const &>(), sampleTime));

  step.process(gateLatch, sampleTime);
}

class InactiveComboStepAvailableToSustain : public EnabledInactiveComboStep {
public:
  void SetUp() override {
    EnabledInactiveComboStep::SetUp();
    setAvailableSteps(false, true);
  }
};

TEST_F(InactiveComboStepAvailableToSustain, process_processesSustainStep) {
  EXPECT_CALL(*sustainStep, process(A<Latch const &>(), sampleTime));

  step.process(gateLatch, sampleTime);
}
