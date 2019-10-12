#include "modules/components/Latch.h"
#include "modules/curve-sequencer/GenerateStep.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mocks/MockStepControls.h>

using dhe::Latch;
using dhe::curve_sequencer::GenerateStep;
using Mode = dhe::curve_sequencer::GenerateStep::Mode;
using State = dhe::curve_sequencer::Step::State;
using dhe::curve_sequencer::Step;

auto constexpr stepIndex = 3;

using ::testing::NiceMock;
using ::testing::Return;

class GenerateStepTest : public ::testing::Test {
protected:
  void givenMode(Mode mode) {
    auto const modeIndex = static_cast<int>(mode);
    ON_CALL(controls, generateMode(stepIndex)).WillByDefault(Return(modeIndex));
  }

  void givenOutput(float voltage) { ON_CALL(controls, output()).WillByDefault(Return(voltage)); }

  void givenLevel(float voltage) { ON_CALL(controls, level(stepIndex)).WillByDefault(Return(voltage)); }

  void givenDuration(float durationSeconds) {
    ON_CALL(controls, duration(stepIndex)).WillByDefault(Return(durationSeconds));
  }

  void givenCurvature(float curvature) { ON_CALL(controls, curvature(stepIndex)).WillByDefault(Return(curvature)); }

  void SetUp() override {
    givenDuration(1.F);
    givenLevel(10.F);
    givenCurvature(0.5F);
    givenOutput(0.F);
  }
  NiceMock<MockStepControls> controls;
  GenerateStep step{controls, stepIndex};
};

TEST_F(GenerateStepTest, processSetsGenerateingLight) {
  EXPECT_CALL(controls, setGenerating(stepIndex, true));

  step.process(Latch{}, 0.F);
}

class GenerateStepSkipMode : public GenerateStepTest {
  void SetUp() override {
    GenerateStepTest::SetUp();
    givenMode(Mode::Skip);
  }
};

TEST_F(GenerateStepSkipMode, isUnvailable) { EXPECT_EQ(step.isAvailable(), false); }

class GenerateStepRiseMode : public GenerateStepTest {
  void SetUp() override {
    GenerateStepTest::SetUp();
    givenMode(Mode::Rise);
  }
};

TEST_F(GenerateStepRiseMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepRiseMode, becomesInactiveIfGateRises) {
  auto const gateLatch = Latch{true, true};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, 0.F);

  EXPECT_EQ(state, State::Inactive);
}

TEST_F(GenerateStepRiseMode, remainsActiveIfGateDoesNotRise) {
  auto const gateLatch = Latch{true, false};

  auto const state = step.process(gateLatch, 0.F);

  EXPECT_EQ(state, State::Active);
}

class GenerateStepFallMode : public GenerateStepTest {
  void SetUp() override {
    GenerateStepTest::SetUp();
    givenMode(Mode::Fall);
  }
};

TEST_F(GenerateStepFallMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepFallMode, becomesInactiveIfGateFalls) {
  auto const gateLatch = Latch{false, true};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, 0.F);

  EXPECT_EQ(state, State::Inactive);
}

TEST_F(GenerateStepFallMode, remainsActiveIfGateDoesNotFall) {
  auto const gateLatch = Latch{false, false};

  auto const state = step.process(gateLatch, 0.F);

  EXPECT_EQ(state, State::Active);
}

class GenerateStepEdgeMode : public GenerateStepTest {
  void SetUp() override {
    GenerateStepTest::SetUp();
    givenMode(Mode::Edge);
  }
};

TEST_F(GenerateStepEdgeMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepEdgeMode, becomesInactiveIfGateRises) {
  auto const gateLatch = Latch{true, true};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, 0.F);

  EXPECT_EQ(state, State::Inactive);
}

TEST_F(GenerateStepEdgeMode, becomesInactiveIfGateFalls) {
  auto const gateLatch = Latch{false, true};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, 0.F);

  EXPECT_EQ(state, State::Inactive);
}

TEST_F(GenerateStepEdgeMode, remainsActiveIfGateHasNoEdge) {
  auto const gateLatch = Latch{false, false};

  auto const state = step.process(gateLatch, 0.F);

  EXPECT_EQ(state, State::Active);
}

class GenerateStepHighMode : public GenerateStepTest {
  void SetUp() override {
    GenerateStepTest::SetUp();
    givenMode(Mode::High);
  }
};

TEST_F(GenerateStepHighMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepHighMode, becomesInactiveIfGateIsHigh) {
  auto const gateLatch = Latch{true, false};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, 0.F);

  EXPECT_EQ(state, State::Inactive);
}

TEST_F(GenerateStepHighMode, remainsActiveIfGateIsLow) {
  auto const gateLatch = Latch{false, false};

  auto const state = step.process(gateLatch, 0.F);

  EXPECT_EQ(state, State::Active);
}

class GenerateStepLowMode : public GenerateStepTest {
  void SetUp() override {
    GenerateStepTest::SetUp();
    givenMode(Mode::Low);
  }
};

TEST_F(GenerateStepLowMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepLowMode, becomesInactiveIfGateIsLow) {
  auto const gateLatch = Latch{false, false};

  EXPECT_CALL(controls, setGenerating(stepIndex, false));

  auto const state = step.process(gateLatch, 0.F);

  EXPECT_EQ(state, State::Inactive);
}

TEST_F(GenerateStepLowMode, remainsActiveIfGateIsHigh) {
  auto const gateLatch = Latch{true, false};

  auto const state = step.process(gateLatch, 0.F);

  EXPECT_EQ(state, State::Active);
}

class GenerateStepDurationMode : public GenerateStepTest {
  void SetUp() override {
    GenerateStepTest::SetUp();
    givenMode(Mode::Duration);
  }
};

TEST_F(GenerateStepDurationMode, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(GenerateStepDurationMode, remainsActiveIfGateRises) {
  auto const gateLatch = Latch{true, true};

  auto const state = step.process(gateLatch, 0.1F);

  EXPECT_EQ(state, State::Active);
}

TEST_F(GenerateStepDurationMode, remainsActiveIfGateFalls) {
  auto const gateLatch = Latch{false, true};

  auto const state = step.process(gateLatch, 0.1F);

  EXPECT_EQ(state, State::Active);
}

TEST_F(GenerateStepDurationMode, remainsActiveIfGateIsHigh) {
  auto const gateLatch = Latch{false, false};

  auto const state = step.process(gateLatch, 0.1F);

  EXPECT_EQ(state, State::Active);
}

TEST_F(GenerateStepDurationMode, remainsActiveIfGateIsLow) {
  auto const gateLatch = Latch{false, false};

  auto const state = step.process(gateLatch, 0.1F);

  EXPECT_EQ(state, State::Active);
}

TEST_F(GenerateStepDurationMode, setsOutputVoltageAccordingToPhase) {
  auto constexpr duration = 1.F; // 1 second
  auto constexpr sampleTime = duration / 2.F;
  auto constexpr rampStartVoltage = 0.F;
  auto constexpr rampEndVoltage = 10.F;
  auto constexpr expectedOutputVoltage = (rampEndVoltage - rampStartVoltage) / 2.F;

  givenOutput(rampStartVoltage);
  givenLevel(rampEndVoltage);
  givenDuration(duration);
  givenCurvature(0.F); // Ramp is linear

  EXPECT_CALL(controls, setOutput(expectedOutputVoltage));

  step.process(Latch{}, sampleTime);
}
