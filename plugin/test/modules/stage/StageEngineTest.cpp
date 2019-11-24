#include "stage/StageEngine.h"

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

  class EventlessMode {
  public:
    MOCK_METHOD(void, enter, (), ());
    MOCK_METHOD(void, execute, (), ());
    MOCK_METHOD(void, exit, (), ());
  };

  class GenerateMode {
  public:
    MOCK_METHOD(void, enter, (), ());
    MOCK_METHOD(dhe::stage::Event, execute, (float), ());
    MOCK_METHOD(void, exit, (), ());
  };

protected:
  NiceMock<Controls> controls{};
  NiceMock<EventlessMode> deferMode{};
  NiceMock<EventlessMode> inputMode{};
  NiceMock<GenerateMode> generateMode{};
  NiceMock<EventlessMode> levelMode{};
  dhe::stage::StageEngine<Controls, EventlessMode, EventlessMode, GenerateMode, EventlessMode> engine{
      controls, deferMode, inputMode, generateMode, levelMode};

  void givenDefer(bool isDeferring) { ON_CALL(controls, isDeferring()).WillByDefault(Return(isDeferring)); }
  void givenInput(float input) { ON_CALL(controls, input()).WillByDefault(Return(input)); }
  void givenTrigger(bool isTriggered) { ON_CALL(controls, isTriggered()).WillByDefault(Return(isTriggered)); }
};

TEST_F(StageEngineTest, startsInInputMode) {
  EXPECT_CALL(inputMode, execute());

  engine.process(0.F);
}

class StageEngineInputMode : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();
    engine.process(0.F);
  }
};

TEST_F(StageEngineInputMode, beginsDeferring_ifDeferRises) {
  givenDefer(true);

  EXPECT_CALL(inputMode, exit());
  EXPECT_CALL(deferMode, enter());
  EXPECT_CALL(deferMode, execute());

  engine.process(0.F);
}

TEST_F(StageEngineInputMode, executesInputMode_ifTriggerDoesNotRiseAndDeferIsLow) {
  givenDefer(false);
  givenTrigger(false);

  EXPECT_CALL(inputMode, execute());

  engine.process(0.F);
}

TEST_F(StageEngineInputMode, beginsGenerating_ifTriggerRisesAndDeferIsLow) {
  givenDefer(false);
  givenTrigger(true);

  auto constexpr input{4.234};
  givenInput(input);

  EXPECT_CALL(inputMode, exit());
  EXPECT_CALL(generateMode, enter());
  EXPECT_CALL(generateMode, execute(A<float>()));

  engine.process(0.F);
}

class StageEngineDeferMode : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();
    givenDefer(true);
    engine.process(0.F);
  }
};

TEST_F(StageEngineDeferMode, executesDeferMode_regardlessOfTrigger_ifDeferIsHigh) {
  givenDefer(true);

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
TEST_F(StageEngineDeferMode, beginsTrackingInput_ifDeferFalls) {
  givenDefer(false);

  EXPECT_CALL(deferMode, exit());
  EXPECT_CALL(inputMode, enter());
  EXPECT_CALL(inputMode, execute());

  engine.process(0.F);
}

class StageEngineGenerateMode : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();
    ON_CALL(generateMode, execute(A<float>())).WillByDefault(Return(Event::Generated));
    givenTrigger(true);
    engine.process(0.F);
    givenTrigger(false);
    engine.process(0.F);
  }
};

TEST_F(StageEngineGenerateMode, beginsDeferring_ifDeferRises) {
  givenDefer(true);

  EXPECT_CALL(generateMode, exit());
  EXPECT_CALL(deferMode, enter());
  EXPECT_CALL(deferMode, execute());

  engine.process(0.F);
}

TEST_F(StageEngineGenerateMode, executesGenerateMode_ifDeferIsLowAndTriggerDoesNotRise) {
  auto constexpr sampleTime{0.04534F};

  givenDefer(false);
  givenTrigger(false);

  EXPECT_CALL(generateMode, execute(sampleTime));

  engine.process(sampleTime);
}

TEST_F(StageEngineGenerateMode, restartsGenerateMode_ifTriggerRisesAndDeferIsLow) {
  givenDefer(false);
  givenTrigger(true);

  EXPECT_CALL(generateMode, exit());
  EXPECT_CALL(generateMode, enter());
  EXPECT_CALL(generateMode, execute(A<float>()));

  engine.process(0.F);
}

TEST_F(StageEngineGenerateMode, raisesEoc_ifDoneGenerating) {
  ON_CALL(generateMode, execute(A<float>())).WillByDefault(Return(Event::Completed));

  EXPECT_CALL(controls, showEoc(true));

  engine.process(0.F);
}

class StageEngineLevelMode : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();

    // Start generating
    givenTrigger(true);
    ON_CALL(generateMode, execute(A<float>())).WillByDefault(Return(Event::Generated));
    engine.process(0.F);

    // Finish generating, which enters TrackingLevel mode
    givenTrigger(false);
    ON_CALL(generateMode, execute(A<float>())).WillByDefault(Return(Event::Completed));
    engine.process(0.F);
  }
};

TEST_F(StageEngineLevelMode, beginsDeferring_ifDeferRises) {
  givenDefer(true);

  EXPECT_CALL(levelMode, exit());
  EXPECT_CALL(deferMode, enter());
  EXPECT_CALL(deferMode, execute());

  engine.process(0.F);
}

TEST_F(StageEngineLevelMode, executesLevelMode_ifTriggerDoesNotRiseAndDeferIsLow) {
  givenDefer(false);
  givenTrigger(false);

  EXPECT_CALL(levelMode, execute());

  engine.process(0.F);
}

TEST_F(StageEngineLevelMode, beginsGenerating_ifTriggerRisesAndDeferIsLow) {
  givenDefer(false);
  givenTrigger(true);

  EXPECT_CALL(levelMode, exit());
  EXPECT_CALL(generateMode, enter());
  EXPECT_CALL(generateMode, execute(A<float>()));

  engine.process(0.F);
}
