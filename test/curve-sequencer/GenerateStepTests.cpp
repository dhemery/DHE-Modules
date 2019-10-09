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
  void setMode(Step::Mode mode) {
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
  void SetUp() override { setMode(Step::Mode::Skip); }
};

TEST_F(GenerateStepSkipMode, isUnvailable) { EXPECT_EQ(step.isAvailable(), false); }

class GenerateStepRiseMode : public GenerateStepTest {
  void SetUp() override { setMode(Step::Mode::Rise); }
};

TEST_F(GenerateStepRiseMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepRiseMode, finishesIfGateRises) {
  Latch gateLatch{true, true};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(GenerateStepRiseMode, continuesIfGateDoesNotRise) {
  Latch gateLatch{true, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

class GenerateStepFallMode : public GenerateStepTest {
  void SetUp() override { setMode(Step::Mode::Fall); }
};

TEST_F(GenerateStepFallMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepFallMode, finishesIfGateFalls) {
  Latch gateLatch{false, true};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(GenerateStepFallMode, continuesIfGateDoesNotFall) {
  Latch gateLatch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

class GenerateStepEdgeMode : public GenerateStepTest {
  void SetUp() override { setMode(Step::Mode::Edge); }
};

TEST_F(GenerateStepEdgeMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepEdgeMode, finishesIfGateRises) {
  Latch gateLatch{true, true};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(GenerateStepEdgeMode, finishesIfGateFalls) {
  Latch gateLatch{false, true};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(GenerateStepEdgeMode, continuesIfGateHasNoEdge) {
  Latch gateLatch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

class GenerateStepHighMode : public GenerateStepTest {
  void SetUp() override { setMode(Step::Mode::High); }
};

TEST_F(GenerateStepHighMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepHighMode, finishesIfGateIsHigh) {
  Latch gateLatch{true, false};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(GenerateStepHighMode, continuesIfGateIsLow) {
  Latch gateLatch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

class GenerateStepLowMode : public GenerateStepTest {
  void SetUp() override { setMode(Step::Mode::Low); }
};

TEST_F(GenerateStepLowMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepLowMode, finishesIfGateIsLow) {
  Latch gateLatch{false, false};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Finished);
}

TEST_F(GenerateStepLowMode, continuesIfGateIsHigh) {
  Latch gateLatch{true, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

class GenerateStepDurationMode : public GenerateStepTest {
  void SetUp() override { setMode(Step::Mode::Duration); }
};

TEST_F(GenerateStepDurationMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepDurationMode, continuesIfGateRises) {
  Latch gateLatch{true, true};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

TEST_F(GenerateStepDurationMode, continuesIfGateFalls) {
  Latch gateLatch{false, true};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

TEST_F(GenerateStepDurationMode, continuesIfGateIsHigh) {
  Latch gateLatch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}

TEST_F(GenerateStepDurationMode, continuesIfGateIsLow) {
  Latch gateLatch{false, false};

  auto const state = step.process(gateLatch, sampleTime);

  EXPECT_EQ(state, Step::State::Running);
}
