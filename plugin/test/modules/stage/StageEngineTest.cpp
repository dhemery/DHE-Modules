#include "stage/StageEngine.h"

#include "components/PhaseTimer.h"
#include "stage/Event.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using Event = dhe::stage::Event;
using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class StageEngineTest : public Test {
  class Controls {
  public:
    MOCK_METHOD(float, input, (), (const));
    MOCK_METHOD(bool, isDeferring, (), (const));
    MOCK_METHOD(bool, isTriggered, (), (const));
    MOCK_METHOD(float, level, (), (const));
    MOCK_METHOD(void, showActive, (bool), ());
    MOCK_METHOD(void, showEoc, (bool), ());
    MOCK_METHOD(void, output, (float), ());
  };

  class DeferMode {
  public:
    MOCK_METHOD(void, enter, (), ());
    MOCK_METHOD(void, execute, (), ());
    MOCK_METHOD(void, exit, (), ());
  };

  class GenerateMode {
  public:
    MOCK_METHOD(void, enter, (float), ());
    MOCK_METHOD(dhe::stage::Event, execute, (float), ());
    MOCK_METHOD(void, exit, (), ());
  };

protected:
  NiceMock<Controls> controls{};
  NiceMock<DeferMode> deferMode{};
  NiceMock<GenerateMode> generateMode{};
  dhe::stage::StageEngine<Controls, DeferMode, GenerateMode> engine{controls, deferMode, generateMode};

  void givenDefer(bool isDeferring) { ON_CALL(controls, isDeferring()).WillByDefault(Return(isDeferring)); }
  void givenInput(float input) { ON_CALL(controls, input()).WillByDefault(Return(input)); }
  void givenLevel(float level) { ON_CALL(controls, level()).WillByDefault(Return(level)); }
  void givenTrigger(bool isTriggered) { ON_CALL(controls, isTriggered()).WillByDefault(Return(isTriggered)); }

  void SetUp() override { Test::SetUp(); }
};

class StageEngineTrackingInput : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();
    engine.process(0.F);
  }
};

TEST_F(StageEngineTrackingInput, reportsInactive) {
  EXPECT_CALL(controls, showActive(false));
  engine.process(0.F);
}

TEST_F(StageEngineTrackingInput, beginsDeferring_ifDeferIsHigh) {
  givenDefer(true);

  EXPECT_CALL(deferMode, enter());
  EXPECT_CALL(deferMode, execute());

  engine.process(0.F);
}

TEST_F(StageEngineTrackingInput, outputsInput_ifTriggerDoesNotRise) {
  auto constexpr input{2.3404F};

  givenInput(input);
  givenTrigger(false);

  EXPECT_CALL(controls, output(input));

  engine.process(0.F);
}

TEST_F(StageEngineTrackingInput, beginsGenerating_ifTriggerRisesAndDeferIsLow) {
  givenDefer(false);
  givenTrigger(true);

  auto constexpr input{4.234};
  givenInput(input);

  auto constexpr sampleTime = 0.1F;

  EXPECT_CALL(generateMode, enter(input));
  EXPECT_CALL(generateMode, execute(sampleTime));

  engine.process(sampleTime);
}

class StageEngineDeferring : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();
    givenDefer(true);
    engine.process(0.F);
  }
};

TEST_F(StageEngineDeferring, executesDeferMode_regardlessOfTrigger) {
  givenTrigger(false);
  EXPECT_CALL(deferMode, execute());
  engine.process(0.F);

  givenTrigger(true);
  EXPECT_CALL(deferMode, execute());
  engine.process(0.F);
}

// TODO: On DEFER fall, StageEngine currently discards the TRIG edge. As a result, tne engine tracks input for this
//  sample, even if TRIG is high. Is this necessary? If DEFER falls and TRIG is high, shouldn't we we immediately start
//  generating? If we don't, TRIG might be low on the next sample, and we've missed the signal to generate.
TEST_F(StageEngineDeferring, beginsTrackingInput_ifDeferFalls) {
  givenDefer(false);    // If no longer deferring...
  engine.process(0.1F); // ... should transition to TrackingInput mode

  // Assert that the following process starts generating, which implies that we are indeed tracking input.
  givenTrigger(true);
  EXPECT_CALL(generateMode, enter(A<float>()));
  EXPECT_CALL(generateMode, execute(A<float>()));

  engine.process(0.1F);
}

class StageEngineGenerating : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();
    ON_CALL(generateMode, execute(A<float>())).WillByDefault(Return(Event::Generated));
    givenTrigger(true);
    engine.process(0.F);
    givenTrigger(false);
    engine.process(0.F);
  }
};

TEST_F(StageEngineGenerating, raisesEoc_ifDoneGenerating) {
  ON_CALL(generateMode, execute(A<float>())).WillByDefault(Return(Event::Completed));

  EXPECT_CALL(controls, showEoc(true));

  engine.process(0.0001F);
}

class StageEngineTrackingLevel : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();

    // Start generating
    givenTrigger(true);
    ON_CALL(generateMode, execute(A<float>())).WillByDefault(Return(Event::Generated));
    engine.process(0.0001F);

    // Finish generating, which enters TrackingLevel mode
    givenTrigger(false);
    ON_CALL(generateMode, execute(A<float>())).WillByDefault(Return(Event::Completed));
    engine.process(0.0001F);
  }
};

TEST_F(StageEngineTrackingLevel, outputsLevel_ifTriggerDoesNotRise) {
  auto constexpr level{4.3498F};
  givenLevel(level);
  givenTrigger(false);

  EXPECT_CALL(controls, output(level));

  engine.process(0.F);
}

TEST_F(StageEngineTrackingLevel, beginsGenerating_ifTriggerRises) {
  givenTrigger(true);
  EXPECT_CALL(generateMode, enter(A<float>()));
  EXPECT_CALL(generateMode, execute(A<float>()));

  engine.process(0.1F);
}
