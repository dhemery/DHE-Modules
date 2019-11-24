#include "stage/StageEngine.h"

#include "components/Latch.h"
#include "stage/Event.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using Event = dhe::stage::Event;
using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class StageEngineTest : public Test {
  class Controls {
  public:
    MOCK_METHOD(bool, defer, (), (const));
    MOCK_METHOD(float, input, (), (const));
    MOCK_METHOD(float, level, (), (const));
    MOCK_METHOD(void, output, (float), ());
    MOCK_METHOD(void, showActive, (bool), ());
    MOCK_METHOD(void, showEoc, (bool), ());
    MOCK_METHOD(bool, trigger, (), (const));
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
    MOCK_METHOD(dhe::stage::Event, execute, (Latch const &, float), ());
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

  void givenDefer(bool defer) { ON_CALL(controls, defer()).WillByDefault(Return(defer)); }
  void givenInput(float input) { ON_CALL(controls, input()).WillByDefault(Return(input)); }
  void givenTrigger(bool trigger) { ON_CALL(controls, trigger()).WillByDefault(Return(trigger)); }
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
  EXPECT_CALL(generateMode, execute(A<Latch const &>(), A<float>()));

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

TEST_F(StageEngineDeferMode, beginsTrackingInput_ifDeferFalls) {
  givenDefer(false);

  EXPECT_CALL(deferMode, exit());
  EXPECT_CALL(inputMode, enter());
  EXPECT_CALL(inputMode, execute());

  engine.process(0.F);
}

/**
 * This is a characterization test for a current quirk: On DEFER fall, the engine pretends that TRIG is low, even if it
 * is high. As a result, when DEFER falls, the engine ignores TRIG for that sample.
 *
 * This behavior is a kludge to deal with what I suspect is a mistake elsewhere in the state machine.
 *
 * The mistake: When DEFER falls, the state machine *always* transitions to TrackingInput, regardness of TRIG state.
 *
 * The kludge: When DEFER falls, pretend that TRIG was low so that, on the subsequent sample, the continuing EOC from
 * the upstream module looks like TRIG rise, and starts this module generating.
 */
TEST_F(StageEngineDeferMode, tracksInputInsteadOfGenerating_ifTrigRisesWhenDeferFalls) {
  givenDefer(true);
  givenTrigger(false);
  engine.process(0.F); // Get trigger to be low while defer is high

  givenTrigger(true); // Generate TRIG rise...
  givenDefer(false);  // ... along with DEFER fall

  EXPECT_CALL(deferMode, exit());
  EXPECT_CALL(inputMode, enter());
  EXPECT_CALL(inputMode, execute());
  EXPECT_CALL(generateMode, enter()).Times(0);
  EXPECT_CALL(generateMode, execute(A<Latch const &>(), A<float>())).Times(0);

  engine.process(0.F);
}

class StageEngineGenerateMode : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();
    ON_CALL(generateMode, execute(A<Latch const &>(), A<float>())).WillByDefault(Return(Event::Generated));
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

  EXPECT_CALL(generateMode, execute(A<Latch const &>(), sampleTime));

  engine.process(sampleTime);
}

TEST_F(StageEngineGenerateMode, passesTriggerStateToGenerateMode_ifDeferIsLowAndTriggerDoesNotRise) {
  givenDefer(false);
  givenTrigger(false);
  engine.process(0.F); // Set latch low

  givenTrigger(false); // Low with no edge
  EXPECT_CALL(generateMode, execute(Latch{false, false}, A<float>()));
  engine.process(0.F);

  givenTrigger(true); // Rise
  EXPECT_CALL(generateMode, execute(Latch{true, true}, A<float>()));
  engine.process(0.F);

  givenTrigger(true); // High with no edge
  EXPECT_CALL(generateMode, execute(Latch{true, false}, A<float>()));
  engine.process(0.F);

  givenTrigger(false); // Fall
  EXPECT_CALL(generateMode, execute(Latch{false, true}, A<float>()));
  engine.process(0.F);
}

TEST_F(StageEngineGenerateMode, raisesEoc_ifDoneGenerating) {
  ON_CALL(generateMode, execute(A<Latch const &>(), A<float>())).WillByDefault(Return(Event::Completed));

  EXPECT_CALL(controls, showEoc(true));

  engine.process(0.F);
}

class StageEngineLevelMode : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();

    // Start generating
    givenTrigger(true);
    ON_CALL(generateMode, execute(A<Latch const &>(), A<float>())).WillByDefault(Return(Event::Generated));
    engine.process(0.F);

    // Finish generating, which enters TrackingLevel mode
    givenTrigger(false);
    ON_CALL(generateMode, execute(A<Latch const &>(), A<float>())).WillByDefault(Return(Event::Completed));
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
  EXPECT_CALL(generateMode, execute(A<Latch const &>(), A<float>()));

  engine.process(0.F);
}
