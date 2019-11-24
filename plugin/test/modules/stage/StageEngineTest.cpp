#include "stage/StageEngine.h"

#include "components/Taper.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>

using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class StageEngineTest : public Test {
  class Controls {
  public:
    MOCK_METHOD(float, curvature, (), (const));
    MOCK_METHOD(float, input, (), (const));
    MOCK_METHOD(float, duration, (), (const));
    MOCK_METHOD(bool, isDeferring, (), (const));
    MOCK_METHOD(bool, isTriggered, (), (const));
    MOCK_METHOD(float, level, (), (const));
    MOCK_METHOD(void, sendActive, (bool), ());
    MOCK_METHOD(void, sendEoc, (bool), ());
    MOCK_METHOD(void, output, (float), ());
    MOCK_METHOD(dhe::taper::VariableTaper const *, taper, (), (const));
  };

protected:
  NiceMock<Controls> controls{};
  dhe::stage::StageEngine<Controls> engine{controls};

  void givenCurvature(float curvature) { ON_CALL(controls, curvature()).WillByDefault(Return(curvature)); }
  void givenDeferring(bool isDeferring) { ON_CALL(controls, isDeferring()).WillByDefault(Return(isDeferring)); }
  void givenDuration(float duration) { ON_CALL(controls, duration()).WillByDefault(Return(duration)); }
  void givenInput(float input) { ON_CALL(controls, input()).WillByDefault(Return(input)); }
  void givenLevel(float level) { ON_CALL(controls, level()).WillByDefault(Return(level)); }
  void givenTriggered(bool isTriggered) { ON_CALL(controls, isTriggered()).WillByDefault(Return(isTriggered)); }
  void givenTaper(dhe::taper::VariableTaper const *taper) { ON_CALL(controls, taper()).WillByDefault(Return(taper)); }

  void SetUp() override {
    Test::SetUp();
    givenTaper(dhe::taper::variableTapers[0]);
    givenDuration(1.F);
  }
};

class StageEngineTrackingInput : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();
    engine.process(0.F);
  }
};

TEST_F(StageEngineTrackingInput, reportsInactive) {
  EXPECT_CALL(controls, sendActive(false));
  engine.process(0.F);
}

TEST_F(StageEngineTrackingInput, outputsInput_ifTriggerDoesNotRise) {
  auto constexpr input{2.3404F};

  givenInput(input);
  givenTriggered(false);

  EXPECT_CALL(controls, output(input));

  engine.process(0.F);
}

TEST_F(StageEngineTrackingInput, beginsGenerating_ifTriggerRises) {
  givenTriggered(true);

  givenCurvature(0.F); // 0 curvature -> linear ramp
  givenDuration(1.F);  // 1s ramp
  givenInput(4.F);     // Start ramp at 4V
  givenLevel(6.F);     // End ramp at 6V

  // Ramp runs from 4V to 6V over 1s
  auto constexpr sampleTime = 0.1F; // So will advance ramp from 4V to 4.2V

  EXPECT_CALL(controls, output(4.2F));

  engine.process(sampleTime); // 0.1s per sample
}

class StageEngineDeferring : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();
    givenDeferring(true);
    engine.process(0.F);
  }
};

TEST_F(StageEngineDeferring, reportsActive) {
  EXPECT_CALL(controls, sendActive(true));
  engine.process(0.F);
}

TEST_F(StageEngineDeferring, outputsInput_regardlessOfTrigger) {
  auto input{2.3404F};
  givenInput(input);
  givenTriggered(false);

  EXPECT_CALL(controls, output(input));
  engine.process(0.F);

  input = 1.783674F;
  givenInput(input);
  givenTriggered(true);

  EXPECT_CALL(controls, output(input));

  engine.process(0.F);
}

// TODO: On DEFER fall, StageEngine currently discards the TRIG edge. As a result, tne engine tracks input for this
//  sample, even if TRIG is high. Is this necessary? If DEFER falls and TRIG is high, shouldn't we we immediately start
//  generating? If we don't, TRIG might be low on the next sample, and we've missed the signal to generate.
TEST_F(StageEngineDeferring, beginsTrackingInput_ifDeferFalls) {
  givenDeferring(false); // If no longer deferring...
  engine.process(0.1F);  // ... should transition to TrackingInput mode

  // The rest of this test asserts that we are in TrackingInput mode.
  // We assert this by sending a rising Trigger, and asserting that we started generating.
  // If we were still Deferring, the trigger would not start generating.
  // TODO: Create a generator component, then use a mock here to make sure we call it.
  givenTriggered(true);

  givenCurvature(0.F); // 0 curvature -> linear ramp
  givenInput(4.F);     // Output ramps from 4V...
  givenLevel(6.F);     // ... to 6V ...
  givenDuration(1.F);  // ... over 1s. That's a 2v increase over 1s.

  auto constexpr sampleTime = 0.1F; // Sample time of 1/10s raises output by 0.2V on this sample, from 4V to 4.2V.

  EXPECT_CALL(controls, output(4.2F));

  engine.process(sampleTime); // 0.1s per sample
}

class StageEngineGenerating : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();
    givenTriggered(true);
    engine.process(0.F);
    givenTriggered(false);
    engine.process(0.F);
  }
};

TEST_F(StageEngineGenerating, raisesEoc_ifDoneGenerating) {
  auto constexpr eocPulseDuration{1e-3F};
  // sample time must be less that EOC pulse, or else the EOC pulse
  // will start, and then immediately finish on the same sample
  auto constexpr sampleTime{eocPulseDuration / 2.F};
  auto constexpr duration{sampleTime * 2.F};

  givenDuration(duration);

  engine.process(sampleTime); // Advance halfway to duration

  EXPECT_CALL(controls, sendEoc(true));

  engine.process(sampleTime); // Advance to end of duration
}

class StageEngineTrackingLevel : public StageEngineTest {};
