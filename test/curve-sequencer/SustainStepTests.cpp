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
  auto const gateLatch = Latch{true, true};

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Inactive);
}

TEST_F(SustainStepRiseMode, continuesIfGateDoesNotRise) {
  auto const gateLatch = Latch{true, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}

class SustainStepFallMode : public SustainStepTest {
  void SetUp() override { givenMode(Step::Mode::Fall); }
};

TEST_F(SustainStepFallMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepFallMode, finishesIfGateFalls) {
  auto const gateLatch = Latch{false, true};

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Inactive);
}

TEST_F(SustainStepFallMode, continuesIfGateDoesNotFall) {
  auto const gateLatch = Latch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}

class SustainStepEdgeMode : public SustainStepTest {
  void SetUp() override { givenMode(Step::Mode::Edge); }
};

TEST_F(SustainStepEdgeMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepEdgeMode, finishesIfGateRises) {
  auto const gateLatch = Latch{true, true};

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Inactive);
}

TEST_F(SustainStepEdgeMode, finishesIfGateFalls) {
  auto const gateLatch = Latch{false, true};

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Inactive);
}

TEST_F(SustainStepEdgeMode, continuesIfGateHasNoEdge) {
  auto const gateLatch = Latch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}

class SustainStepHighMode : public SustainStepTest {
  void SetUp() override { givenMode(Step::Mode::High); }
};

TEST_F(SustainStepHighMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepHighMode, finishesIfGateIsHigh) {
  auto const gateLatch = Latch{true, false};

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Inactive);
}

TEST_F(SustainStepHighMode, continuesIfGateIsLow) {
  auto const gateLatch = Latch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}

class SustainStepLowMode : public SustainStepTest {
  void SetUp() override { givenMode(Step::Mode::Low); }
};

TEST_F(SustainStepLowMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(SustainStepLowMode, finishesIfGateIsLow) {
  auto const gateLatch = Latch{false, false};

  EXPECT_CALL(controls, setSustaining(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Inactive);
}

TEST_F(SustainStepLowMode, continuesIfGateIsLow) {
  auto const gateLatch = Latch{true, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}
