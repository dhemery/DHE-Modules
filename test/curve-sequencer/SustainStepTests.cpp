#include "modules/components/Latch.h"
#include "modules/curve-sequencer/StepControls.h"
#include "modules/curve-sequencer/StepMode.h"
#include "modules/curve-sequencer/SustainStep.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::StepControls;
using dhe::curve_sequencer::StepMode;
using dhe::curve_sequencer::SustainStep;

struct MockStepControls : public StepControls {
  MOCK_METHOD(bool, isEnabled, (int), (const, override));
  MOCK_METHOD(StepMode, generateMode, (int), (const, override));
  MOCK_METHOD(void, setGenerating, (int, bool), (override));
  MOCK_METHOD(StepMode, sustainMode, (int), (const, override));
  MOCK_METHOD(void, setSustaining, (int, bool), (override));
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

class SustainStepTest : public ::testing::Test {
public:
  NiceMock<MockStepControls> controls;
  NiceMock<MockLatch> gateLatch;

  SustainStep step{controls, stepIndex};

  void setMode(StepMode mode) { ON_CALL(controls, sustainMode(stepIndex)).WillByDefault(Return(mode)); }

  void setGate(bool state, bool edge) {
    ON_CALL(gateLatch, isHigh()).WillByDefault(Return(state));
    ON_CALL(gateLatch, isEdge()).WillByDefault(Return(edge));
  }
};

TEST_F(SustainStepTest, processSetsSustainingLight) {
  EXPECT_CALL(controls, setSustaining(stepIndex, true));

  step.process(gateLatch, sampleTime);
}

class SustainStepSkipMode : public SustainStepTest {
  void SetUp() override { setMode(StepMode::Skip); }
};

TEST_F(SustainStepSkipMode, isUnvailable) { EXPECT_EQ(step.isAvailable(), false); }

class SustainStepRiseMode : public SustainStepTest {
  void SetUp() override { setMode(StepMode::Rise); }
};

TEST_F(SustainStepRiseMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepRiseMode, finishesIfGateRises) {
  setGate(true, true);

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  step.process(gateLatch, sampleTime);
}

class SustainStepFallMode : public SustainStepTest {
  void SetUp() override { setMode(StepMode::Fall); }
};

TEST_F(SustainStepFallMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepFallMode, finishesIfGateFalls) {
  setGate(false, true);

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  step.process(gateLatch, sampleTime);
}
