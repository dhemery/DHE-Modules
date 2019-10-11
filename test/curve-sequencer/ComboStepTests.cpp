#include "modules/components/Latch.h"
#include "modules/curve-sequencer/ComboStep.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mocks/MockStep.h>
#include <mocks/MockStepControls.h>

using dhe::Latch;
using dhe::curve_sequencer::ComboStep;
using dhe::curve_sequencer::Step;

auto constexpr sampleTime = 1.F / 44000.F;
auto constexpr stepIndex = 3;

using ::testing::A;
using ::testing::Return;

class ComboStepDisabledIdle : public ::testing::Test {
protected:
  void SetUp() override { givenEnabled(false); }

  void givenEnabled(bool enabled) { ON_CALL(controls, isEnabled(stepIndex)).WillByDefault(Return(enabled)); }

  void givenGenerateAvailable(bool isAvailable, Step::State state = Step::State::Active) {
    ON_CALL(*generateStep, isAvailable()).WillByDefault(Return(isAvailable));
    givenGenerateReturns(state);
  }

  void givenGenerateReturns(Step::State state) {
    ON_CALL(*generateStep, process(A<Latch const &>(), A<float>())).WillByDefault(Return(state));
  }

  void givenSustainAvailable(bool isAvailable, Step::State state = Step::State::Active) {
    ON_CALL(*sustainStep, isAvailable()).WillByDefault(Return(isAvailable));
    givenSustainReturns(state);
  }

  void givenSustainReturns(Step::State state) {
    ON_CALL(*sustainStep, process(A<Latch const &>(), A<float>())).WillByDefault(Return(state));
  }

  MockStepControls controls;
  MockStep *generateStep = new MockStep;
  MockStep *sustainStep = new MockStep;

  ComboStep step{controls, stepIndex, generateStep, sustainStep};
};

TEST_F(ComboStepDisabledIdle, isUnavailable) { EXPECT_EQ(step.isAvailable(), false); }

class ComboStepEnabledIdle : public ComboStepDisabledIdle {
protected:
  void SetUp() override {
    ComboStepDisabledIdle::SetUp();
    givenEnabled(true);
  }
};

class ComboStepEnabledIdleGenerateUnavailableSustainUnavailable : public ComboStepEnabledIdle {
protected:
  void SetUp() override {
    ComboStepEnabledIdle::SetUp();
    givenGenerateAvailable(false);
    givenSustainAvailable(false);
  }
};

TEST_F(ComboStepEnabledIdleGenerateUnavailableSustainUnavailable, isUnavailable) {
  EXPECT_EQ(step.isAvailable(), false);
}

class ComboStepEnabledIdleGenerateAvailableSustainUnavailable : public ComboStepEnabledIdle {
protected:
  void SetUp() override {
    ComboStepEnabledIdle::SetUp();
    givenGenerateAvailable(true);
    givenSustainAvailable(false);
  }
};

TEST_F(ComboStepEnabledIdleGenerateAvailableSustainUnavailable, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(ComboStepEnabledIdleGenerateAvailableSustainUnavailable, process_generates) {
  EXPECT_CALL(*generateStep, process(A<Latch const &>(), sampleTime));
  EXPECT_CALL(*sustainStep, process(A<Latch const &>(), sampleTime)).Times(0);

  step.process(Latch{}, sampleTime);
}

class ComboStepEnabledIdleGenerateUnavailableSustainAvailable : public ComboStepEnabledIdle {
protected:
  void SetUp() override {
    ComboStepEnabledIdle::SetUp();
    givenGenerateAvailable(false);
    givenSustainAvailable(true);
  }
};

TEST_F(ComboStepEnabledIdleGenerateUnavailableSustainAvailable, process_sustains) {
  EXPECT_CALL(*sustainStep, process(A<Latch const &>(), sampleTime));
  EXPECT_CALL(*generateStep, process(A<Latch const &>(), sampleTime)).Times(0);

  step.process(Latch{}, sampleTime);
}

class ComboStepEnabledIdleGenerateAvailableSustainAvailable : public ComboStepEnabledIdle {
protected:
  void SetUp() override {
    ComboStepEnabledIdle::SetUp();
    givenGenerateAvailable(true);
    givenSustainAvailable(true);
  }
};

TEST_F(ComboStepEnabledIdleGenerateAvailableSustainAvailable, isAvailable) { EXPECT_EQ(step.isAvailable(), true); }

TEST_F(ComboStepEnabledIdleGenerateAvailableSustainAvailable, process_generates) {
  EXPECT_CALL(*generateStep, process(A<Latch const &>(), sampleTime));
  EXPECT_CALL(*sustainStep, process(A<Latch const &>(), sampleTime)).Times(0);

  step.process(Latch{}, sampleTime);
}

class ComboStepEnabledGeneratingGenerateAvailableSustainUnavailable
    : public ComboStepEnabledIdleGenerateAvailableSustainUnavailable {
protected:
  void SetUp() override {
    ComboStepEnabledIdleGenerateAvailableSustainUnavailable::SetUp();

    // Begin generating
    step.process(Latch{}, sampleTime);
  }
};

TEST_F(ComboStepEnabledGeneratingGenerateAvailableSustainUnavailable, isAvailable) {
  EXPECT_EQ(step.isAvailable(), true);
}

TEST_F(ComboStepEnabledGeneratingGenerateAvailableSustainUnavailable, process_generates) {
  EXPECT_CALL(*generateStep, process(A<Latch const &>(), sampleTime));
  EXPECT_CALL(*sustainStep, process(A<Latch const &>(), sampleTime)).Times(0);

  step.process(Latch{}, sampleTime);
}

class ComboStepEnabledGeneratingGenerateAvailableSustainAvailable
    : public ComboStepEnabledIdleGenerateAvailableSustainAvailable {
protected:
  void SetUp() override {
    ComboStepEnabledIdleGenerateAvailableSustainAvailable::SetUp();

    // Begin generating
    step.process(Latch{}, sampleTime);
  }
};

TEST_F(ComboStepEnabledGeneratingGenerateAvailableSustainAvailable, isAvailable) {
  EXPECT_EQ(step.isAvailable(), true);
}

TEST_F(ComboStepEnabledGeneratingGenerateAvailableSustainAvailable, process_generates) {
  EXPECT_CALL(*generateStep, process(A<Latch const &>(), sampleTime));
  EXPECT_CALL(*sustainStep, process(A<Latch const &>(), sampleTime)).Times(0);

  step.process(Latch{}, sampleTime);
}

class ComboStepGenerating : public ComboStepEnabledIdle {
protected:
  void SetUp() override {
    ComboStepEnabledIdle::SetUp();
    givenGenerateAvailable(true);

    // Begin generating
    step.process(Latch{}, sampleTime);
  }
};

TEST_F(ComboStepGenerating, ifGenerateReturnsActive_processReturnsActive) {
  givenGenerateReturns(Step::State::Active);

  auto const state = step.process(Latch{}, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}

TEST_F(ComboStepGenerating, ifGenerateReturnsActive_nextProcessGenerates) {
  givenGenerateReturns(Step::State::Active);

  step.process(Latch{}, sampleTime);

  EXPECT_CALL(*generateStep, process(A<Latch const &>(), sampleTime));

  step.process(Latch{}, sampleTime);
}

TEST_F(ComboStepGenerating, ifGenerateReturnsInactive_andSustainIsAvailable_processReturnsActive) {
  givenGenerateReturns(Step::State::Inactive);
  givenSustainAvailable(true);

  auto const state = step.process(Latch{}, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}

TEST_F(ComboStepGenerating, ifGenerateReturnsInactive_andSustainIsAvailable_nextProcessSustains) {
  givenGenerateReturns(Step::State::Inactive);
  givenSustainAvailable(true);

  step.process(Latch{}, sampleTime);

  EXPECT_CALL(*generateStep, process(A<Latch const &>(), sampleTime)).Times(0);
  EXPECT_CALL(*sustainStep, process(A<Latch const &>(), sampleTime));

  step.process(Latch{}, sampleTime);
}

TEST_F(ComboStepGenerating, ifGenerateReturnsInactive_andSustainIsUnavailable_processReturnsInactive) {
  givenGenerateReturns(Step::State::Inactive);
  givenSustainAvailable(false);

  auto const state = step.process(Latch{}, sampleTime);

  EXPECT_EQ(state, Step::State::Inactive);
}

TEST_F(ComboStepGenerating, isAvailable_evenIfDisabled) {
  givenEnabled(false);

  EXPECT_EQ(step.isAvailable(), true);
}

TEST_F(ComboStepGenerating, generates_evenIfDisabled) {
  givenEnabled(false);

  EXPECT_CALL(*generateStep, process(A<Latch const &>(), sampleTime));

  step.process(Latch{}, sampleTime);
}


TEST_F(ComboStepGenerating, isAvailable_evenIfGenerateIsUnavailable) {
  givenGenerateAvailable(false);

  EXPECT_EQ(step.isAvailable(), true);
}

TEST_F(ComboStepGenerating, generates_evenIfGenerateIsUnavailable) {
  givenGenerateAvailable(false);

  EXPECT_CALL(*generateStep, process(A<Latch const &>(), sampleTime));

  step.process(Latch{}, sampleTime);
}

class ComboStepSustaining : public ComboStepEnabledIdle {
protected:
  void SetUp() override {
    ComboStepEnabledIdle::SetUp();
    givenSustainAvailable(true);

    // Begin sustaining
    step.process(Latch{}, sampleTime);
  }
};

TEST_F(ComboStepSustaining, ifSustainReturnsActive_processReturnsActive) {
  givenSustainReturns(Step::State::Active);

  auto const state = step.process(Latch{}, sampleTime);

  EXPECT_EQ(state, Step::State::Active);
}

TEST_F(ComboStepSustaining, ifSustainReturnsActive_nextProcessSustains) {
  givenSustainReturns(Step::State::Active);

  step.process(Latch{}, sampleTime);

  EXPECT_CALL(*sustainStep, process(A<Latch const &>(), sampleTime));

  step.process(Latch{}, sampleTime);
}

TEST_F(ComboStepSustaining, ifSustainReturnsInactive_processReturnsInactive) {
  givenSustainReturns(Step::State::Inactive);

  auto const state = step.process(Latch{}, sampleTime);

  EXPECT_EQ(state, Step::State::Inactive);
}

TEST_F(ComboStepSustaining, isAvailable_evenIfDisabled) {
  givenEnabled(false);

  EXPECT_EQ(step.isAvailable(), true);
}

TEST_F(ComboStepSustaining, sustains_evenIfDisabled) {
  givenEnabled(false);

  EXPECT_CALL(*sustainStep, process(A<Latch const &>(), sampleTime));

  step.process(Latch{}, sampleTime);
}


TEST_F(ComboStepSustaining, isAvailable_evenIfSustainIsUnavailable) {
  givenSustainAvailable(false);

  EXPECT_EQ(step.isAvailable(), true);
}

TEST_F(ComboStepSustaining, sustains_evenIfSustainIsUnavailable) {
  givenSustainAvailable(false);

  EXPECT_CALL(*sustainStep, process(A<Latch const &>(), sampleTime));

  step.process(Latch{}, sampleTime);
}
