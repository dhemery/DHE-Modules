#include "modules/curve-sequencer/Step.h"
#include "modules/curve-sequencer/SustainStep.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mocks/MockStepControls.h>

using dhe::Latch;
using dhe::curve_sequencer::Step;
using dhe::curve_sequencer::SustainStep;

auto constexpr sampleTime = 1.F / 44000.F;
auto constexpr stepIndex = 3;

using ::testing::NiceMock;
using ::testing::Return;

class SustainStepTest : public ::testing::Test {
protected:
  void givenMode(Step::Mode mode) {
    auto const modeIndex = static_cast<int>(mode);
    ON_CALL(controls, sustainMode(stepIndex)).WillByDefault(Return(modeIndex));
  }

  NiceMock<MockStepControls> controls;
  SustainStep step{controls, stepIndex};
};

TEST_F(SustainStepTest, processSetsSustainingLight) {
  EXPECT_CALL(controls, setSustaining(stepIndex, true));

  step.process(Latch{}, sampleTime);
}

class SustainStepSkipMode : public SustainStepTest {
  void SetUp() override { givenMode(Step::Mode::Skip); }
};

TEST_F(SustainStepSkipMode, isUnvailable) { EXPECT_EQ(step.isAvailable(), false); }

class SustainStepRiseMode : public SustainStepTest {
  void SetUp() override { givenMode(Step::Mode::Rise); }
};

TEST_F(SustainStepRiseMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepRiseMode, finishesIfGateRises) {
  Latch gateLatch{true, true};

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(SustainStepRiseMode, continuesIfGateDoesNotRise) {
  Latch gateLatch{true, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

class SustainStepFallMode : public SustainStepTest {
  void SetUp() override { givenMode(Step::Mode::Fall); }
};

TEST_F(SustainStepFallMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepFallMode, finishesIfGateFalls) {
  Latch gateLatch{false, true};

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(SustainStepFallMode, continuesIfGateDoesNotFall) {
  Latch gateLatch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

class SustainStepEdgeMode : public SustainStepTest {
  void SetUp() override { givenMode(Step::Mode::Edge); }
};

TEST_F(SustainStepEdgeMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepEdgeMode, finishesIfGateRises) {
  Latch gateLatch{true, true};

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(SustainStepEdgeMode, finishesIfGateFalls) {
  Latch gateLatch{false, true};

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(SustainStepEdgeMode, continuesIfGateHasNoEdge) {
  Latch gateLatch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

class SustainStepHighMode : public SustainStepTest {
  void SetUp() override { givenMode(Step::Mode::High); }
};

TEST_F(SustainStepHighMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepHighMode, finishesIfGateIsHigh) {
  Latch gateLatch{true, false};

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(SustainStepHighMode, continuesIfGateIsLow) {
  Latch gateLatch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

class SustainStepLowMode : public SustainStepTest {
  void SetUp() override { givenMode(Step::Mode::Low); }
};

TEST_F(SustainStepLowMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepLowMode, finishesIfGateIsLow) {
  Latch gateLatch{false, false};

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(SustainStepLowMode, continuesIfGateIsLow) {
  Latch gateLatch{true, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}
