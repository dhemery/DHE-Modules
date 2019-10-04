#include "modules/components/Latch.h"
#include "modules/curve-sequencer/GenerateStep.h"
#include "modules/curve-sequencer/StepControls.h"
#include "modules/curve-sequencer/StepMode.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::GenerateStep;
using dhe::curve_sequencer::StepControls;
using StepMode = dhe::curve_sequencer::StepMode;

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

class GenerateStepTest : public ::testing::Test {
public:
  NiceMock<MockStepControls> controls;

  GenerateStep step{controls, stepIndex};
  NiceMock<MockLatch> gateLatch;

  void setMode(StepMode mode) { ON_CALL(controls, generateMode(stepIndex)).WillByDefault(Return(mode)); }
};

TEST_F(GenerateStepTest, isUnvailableInSkipMode) {
  setMode(StepMode::Skip);

  EXPECT_EQ(step.isAvailable(), false);
}

TEST_F(GenerateStepTest, isAvailableIfNotInSkipMode) {
  setMode(StepMode::Rise);

  EXPECT_EQ(step.isAvailable(), true);
}

TEST_F(GenerateStepTest, processSetsGeneratingLight) {
  EXPECT_CALL(controls, setGenerating(stepIndex, true));

  step.process(gateLatch, sampleTime);
}
