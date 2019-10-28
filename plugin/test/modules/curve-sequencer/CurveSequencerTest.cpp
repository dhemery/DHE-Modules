#include "curve-sequencer/CurveSequencer.h"

#include "components/Latch.h"
#include "curve-sequencer/CurveSequencerControls.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <mocks/MockLight.h>
#include <mocks/MockParam.h>
#include <mocks/MockPort.h>
#include <vector>

static auto constexpr defaultSampleTime = 1.F / 44100.F;
static auto constexpr stepCount = 4;

using dhe::Latch;
using dhe::curve_sequencer::CurveSequencer;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;

using ::testing::A;
using ::testing::An;
using ::testing::NiceMock;
using ::testing::Return;

using InputType = NiceMock<MockPort>;
using OutputType = NiceMock<MockPort>;
using ParamType = NiceMock<MockParam>;
using LightType = NiceMock<MockLight>;

class MockStepExecutor {
public:
  MOCK_METHOD(bool, execute, (int, Latch const &, float) );
};

class NewSequence : public ::testing::Test {
  using CurveSequencer = CurveSequencer<4, InputType, OutputType, ParamType, LightType, MockStepExecutor>;

protected:
  std::vector<InputType> inputs{Controls::InputCount};
  std::vector<OutputType> outputs{Controls::OutputCount};
  std::vector<ParamType> params{Controls::ParameterCount};
  std::vector<LightType> lights{Controls::LightCount};
  MockStepExecutor *stepExecutor = new NiceMock<MockStepExecutor>{};
  CurveSequencer curveSequencer{inputs, outputs, params, lights, stepExecutor};

  void givenRunInput(bool state) {
    ON_CALL(inputs[Controls::RunInput], getVoltage()).WillByDefault(Return(state ? 10.F : 0.F));
  }

  void givenGateInput(bool state) {
    ON_CALL(inputs[Controls::GateInput], getVoltage()).WillByDefault(Return(state ? 10.F : 0.F));
  }

  void givenSelection(int start, int length) {
    ON_CALL(params[Controls::StartKnob], getValue()).WillByDefault(Return(static_cast<float>(start)));
    ON_CALL(params[Controls::StepsKnob], getValue()).WillByDefault(Return(static_cast<float>(length)));
  }

  void givenActiveSteps(std::vector<int> const &indices) {
    for (auto const index : indices) {
      ON_CALL(*stepExecutor, execute(index, A<Latch const &>(), A<float>())).WillByDefault(Return(true));
    }
  }

  void failIfAnyStepExecutesUnexpectedly() {
    EXPECT_CALL(*stepExecutor, execute(An<int>(), A<Latch const &>(), A<float>())).Times(0);
  }

  void SetUp() override {
    for (int i = 0; i < stepCount; i++) {
      givenSelection(0, stepCount);
    }
  };
};

class SequenceNotRunning : public NewSequence {
protected:
  void SetUp() override {
    NewSequence::SetUp();
    givenRunInput(false);
    curveSequencer.execute(0.F);
    failIfAnyStepExecutesUnexpectedly();
  }
};

TEST_F(SequenceNotRunning, executesNoSteps) {
  EXPECT_CALL(*stepExecutor, execute(An<int>(), A<Latch const &>(), A<float>())).Times(0);

  curveSequencer.execute(0.1F);
}

class SequenceIdle : public NewSequence {
protected:
  void SetUp() override {
    NewSequence::SetUp();
    givenRunInput(true);
    givenGateInput(false);
    curveSequencer.execute(0.F);
    failIfAnyStepExecutesUnexpectedly();
  }
};

TEST_F(SequenceIdle, gateRise_executesEachStepFromFirstSelectedStepThroughFirstActiveStep) {
  givenSelection(0, stepCount);

  givenGateInput(true); // Will trigger start of sequence

  EXPECT_CALL(*stepExecutor, execute(0, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  EXPECT_CALL(*stepExecutor, execute(1, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  EXPECT_CALL(*stepExecutor, execute(2, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  EXPECT_CALL(*stepExecutor, execute(3, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(true));

  curveSequencer.execute(defaultSampleTime);
}

TEST_F(SequenceIdle, continuesExecutingFromStep0_ifNoActiveStepAboveFirstSelectedStep) {
  auto constexpr firstActiveStep = 3;
  auto constexpr selectionStart = 6; // Will have to wrap to reach first active step
  givenSelection(selectionStart, stepCount);

  // Will execute selectionStart and higher, which will report inactive
  for (int step = selectionStart; step < stepCount; step++) {
    EXPECT_CALL(*stepExecutor, execute(step, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  }

  // Then will execute step 0 through the step before the first active step, which will report inactive
  for (int step = 0; step < firstActiveStep; step++) {
    EXPECT_CALL(*stepExecutor, execute(step, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  }

  // Finally, will execute the first active step, then stop
  EXPECT_CALL(*stepExecutor, execute(firstActiveStep, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(true));

  givenGateInput(true); // Will trigger start of sequence

  curveSequencer.execute(defaultSampleTime);
}

TEST_F(SequenceIdle, executesEachSelectedStepExactlyOnce_ifNoActiveStep) {
  auto constexpr selectionStart = 0;
  auto constexpr selectionLength = stepCount;
  givenSelection(selectionStart, selectionLength);

  for (int step = selectionStart; step < selectionStart + selectionLength; step++) {
    EXPECT_CALL(*stepExecutor, execute(step, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  }

  givenGateInput(true); // Will trigger start of sequence

  curveSequencer.execute(defaultSampleTime);
}

TEST_F(SequenceIdle, executesOnlySelectedSteps) {
  auto constexpr selectionStart = 3;
  auto constexpr selectionLength = 2;
  givenSelection(selectionStart, selectionLength);

  for (int step = selectionStart; step < selectionStart + selectionLength; step++) {
    EXPECT_CALL(*stepExecutor, execute(step, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  }

  givenGateInput(true); // Will trigger start of sequence

  curveSequencer.execute(defaultSampleTime);
}

class SequenceActive : public NewSequence {
protected:
  static auto constexpr previouslyActiveStep = 3;

  void SetUp() override {
    NewSequence::SetUp();

    givenSelection(0, stepCount);
    givenRunInput(true);
    givenGateInput(true);
    givenActiveSteps({previouslyActiveStep});

    curveSequencer.execute(0.F);
    failIfAnyStepExecutesUnexpectedly();
  }
};

TEST_F(SequenceActive, executesOnlyPreviouslyActiveStep_ifPreviouslyActiveStepIsStillActive) {
  givenRunInput(true);

  EXPECT_CALL(*stepExecutor, execute(previouslyActiveStep, A<Latch const &>(), defaultSampleTime))
      .WillOnce(Return(true));

  curveSequencer.execute(defaultSampleTime);
}

TEST_F(SequenceActive, executesSucessorSteps_ifPreviouslyActiveStepIsNoLongerActive) {
  givenRunInput(true);

  auto constexpr firstActiveSuccessorStep = previouslyActiveStep + 3;

  for (int step = previouslyActiveStep; step < firstActiveSuccessorStep; step++) {
    EXPECT_CALL(*stepExecutor, execute(step, A<Latch const &>(), defaultSampleTime)).WillOnce(Return(false));
  }

  EXPECT_CALL(*stepExecutor, execute(firstActiveSuccessorStep, A<Latch const &>(), defaultSampleTime))
      .WillOnce(Return(true));

  curveSequencer.execute(defaultSampleTime);
}
