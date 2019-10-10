#include "modules/components/Latch.h"
#include "modules/curve-sequencer/GenerateStep.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mocks/MockStepControls.h>

using dhe::curve_sequencer::GenerateStep;
using dhe::curve_sequencer::Step;
using dhe::Latch;

auto constexpr sampleTime = 1.F / 44000.F;
auto constexpr stepIndex = 3;

using ::testing::NiceMock;
using ::testing::Return;

class GenerateStepTest : public ::testing::Test {
protected:
  void givenMode(Step::Mode mode) {
    auto const modeIndex = static_cast<int>(mode);
    ON_CALL(controls, generateMode(stepIndex)).WillByDefault(Return(modeIndex));
  }

  NiceMock<MockStepControls> controls;
  GenerateStep step{controls, stepIndex};

};

TEST_F(GenerateStepTest, processSetsGenerateingLight) {
  EXPECT_CALL(controls, setGenerating(stepIndex, true));

  step.process(Latch{}, sampleTime);
}

class GenerateStepSkipMode : public GenerateStepTest {
  void SetUp() override { givenMode(Step::Mode::Skip); }
};

TEST_F(GenerateStepSkipMode, isUnvailable) { EXPECT_EQ(step.isAvailable(), false); }

class GenerateStepRiseMode : public GenerateStepTest {
  void SetUp() override { givenMode(Step::Mode::Rise); }
};

TEST_F(GenerateStepRiseMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepRiseMode, finishesIfGateRises) {
  auto const gateLatch = Latch{true, true};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Terminated);
}

TEST_F(GenerateStepRiseMode, continuesIfGateDoesNotRise) {
  auto const gateLatch = Latch{true, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}

class GenerateStepFallMode : public GenerateStepTest {
  void SetUp() override { givenMode(Step::Mode::Fall); }
};

TEST_F(GenerateStepFallMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepFallMode, finishesIfGateFalls) {
  auto const gateLatch = Latch{false, true};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Terminated);
}

TEST_F(GenerateStepFallMode, continuesIfGateDoesNotFall) {
  auto const gateLatch = Latch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}

class GenerateStepEdgeMode : public GenerateStepTest {
  void SetUp() override { givenMode(Step::Mode::Edge); }
};

TEST_F(GenerateStepEdgeMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepEdgeMode, finishesIfGateRises) {
  auto const gateLatch = Latch{true, true};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Terminated);
}

TEST_F(GenerateStepEdgeMode, finishesIfGateFalls) {
  auto const gateLatch = Latch{false, true};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Terminated);
}

TEST_F(GenerateStepEdgeMode, continuesIfGateHasNoEdge) {
  auto const gateLatch = Latch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}

class GenerateStepHighMode : public GenerateStepTest {
  void SetUp() override { givenMode(Step::Mode::High); }
};

TEST_F(GenerateStepHighMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepHighMode, finishesIfGateIsHigh) {
  auto const gateLatch = Latch{true, false};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Terminated);
}

TEST_F(GenerateStepHighMode, continuesIfGateIsLow) {
  auto const gateLatch = Latch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}

class GenerateStepLowMode : public GenerateStepTest {
  void SetUp() override { givenMode(Step::Mode::Low); }
};

TEST_F(GenerateStepLowMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepLowMode, finishesIfGateIsLow) {
  auto const gateLatch = Latch{false, false};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Terminated);
}

TEST_F(GenerateStepLowMode, continuesIfGateIsHigh) {
  auto const gateLatch = Latch{true, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}

class GenerateStepDurationMode : public GenerateStepTest {
  void SetUp() override { givenMode(Step::Mode::Duration); }
};

TEST_F(GenerateStepDurationMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepDurationMode, continuesIfGateRises) {
  auto const gateLatch = Latch{true, true};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}

TEST_F(GenerateStepDurationMode, continuesIfGateFalls) {
  auto const gateLatch = Latch{false, true};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}

TEST_F(GenerateStepDurationMode, continuesIfGateIsHigh) {
  auto const gateLatch = Latch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}

TEST_F(GenerateStepDurationMode, continuesIfGateIsLow) {
  auto const gateLatch = Latch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}
