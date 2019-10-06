#include "modules/curve-sequencer/Step.h"
#include "modules/curve-sequencer/SustainStep.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mocks/MockLatch.h>
#include <mocks/MockStepControls.h>

using dhe::curve_sequencer::Step;
using dhe::curve_sequencer::SustainStep;

auto constexpr sampleTime = 1.F / 44000.F;
auto constexpr stepIndex = 3;

using ::testing::NiceMock;
using ::testing::Return;

struct SustainStepTest : public ::testing::Test {
  NiceMock<MockStepControls> controls;
  NiceMock<MockLatch> gateLatch;

  SustainStep step{controls, stepIndex};

  void setMode(Step::Mode mode) {
    auto const modeIndex = static_cast<int>(mode);
    ON_CALL(controls, sustainMode(stepIndex)).WillByDefault(Return(modeIndex));
  }

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
  void SetUp() override { setMode(Step::Mode::Skip); }
};

TEST_F(SustainStepSkipMode, isUnvailable) { EXPECT_EQ(step.isAvailable(), false); }

class SustainStepRiseMode : public SustainStepTest {
  void SetUp() override { setMode(Step::Mode::Rise); }
};

TEST_F(SustainStepRiseMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepRiseMode, finishesIfGateRises) {
  setGate(true, true);

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(SustainStepRiseMode, continuesIfGateDoesNotRise) {
  setGate(true, false);

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

class SustainStepFallMode : public SustainStepTest {
  void SetUp() override { setMode(Step::Mode::Fall); }
};

TEST_F(SustainStepFallMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepFallMode, finishesIfGateFalls) {
  setGate(false, true);

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(SustainStepFallMode, continuesIfGateDoesNotFall) {
  setGate(false, false);

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

class SustainStepEdgeMode : public SustainStepTest {
  void SetUp() override { setMode(Step::Mode::Edge); }
};

TEST_F(SustainStepEdgeMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepEdgeMode, finishesIfGateRises) {
  setGate(true, true);

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(SustainStepEdgeMode, finishesIfGateFalls) {
  setGate(false, true);

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(SustainStepEdgeMode, continuesIfGateHasNoEdge) {
  setGate(false, false);

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

class SustainStepHighMode : public SustainStepTest {
  void SetUp() override { setMode(Step::Mode::High); }
};

TEST_F(SustainStepHighMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepHighMode, finishesIfGateIsHigh) {
  setGate(true, false);

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(SustainStepHighMode, continuesIfGateIsLow) {
  setGate(false, false);

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

class SustainStepLowMode : public SustainStepTest {
  void SetUp() override { setMode(Step::Mode::Low); }
};

TEST_F(SustainStepLowMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepLowMode, finishesIfGateIsLow) {
  setGate(false, false);

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(SustainStepLowMode, continuesIfGateIsLow) {
  setGate(true, false);

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}
