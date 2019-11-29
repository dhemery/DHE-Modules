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
    MOCK_METHOD(bool, gate, (), (const));
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
  dhe::stage::StageEngine<Controls, EventlessMode, EventlessMode, GenerateMode, EventlessMode> stageEngine{
      controls, deferMode, inputMode, generateMode, levelMode};

  void givenDefer(bool defer) { ON_CALL(controls, defer()).WillByDefault(Return(defer)); }
  void givenInput(float input) { ON_CALL(controls, input()).WillByDefault(Return(input)); }
  void givenGate(bool gate) { ON_CALL(controls, gate()).WillByDefault(Return(gate)); }
};

TEST_F(StageEngineTest, startsInInputMode) {
  EXPECT_CALL(inputMode, execute());

  stageEngine.process(0.F);
}

class StageEngineInputMode : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();
    stageEngine.process(0.F);
  }
};

TEST_F(StageEngineInputMode, beginsDeferring_ifDeferRises) {
  givenDefer(true);

  EXPECT_CALL(inputMode, exit());
  EXPECT_CALL(deferMode, enter());
  EXPECT_CALL(deferMode, execute());

  stageEngine.process(0.F);
}

TEST_F(StageEngineInputMode, executesInputMode_ifGateDoesNotRiseAndDeferIsLow) {
  givenDefer(false);
  givenGate(false);

  EXPECT_CALL(inputMode, execute());

  stageEngine.process(0.F);
}

TEST_F(StageEngineInputMode, beginsGenerating_ifGateRisesAndDeferIsLow) {
  givenDefer(false);
  givenGate(true);

  auto constexpr input{4.234};
  givenInput(input);

  EXPECT_CALL(inputMode, exit());
  EXPECT_CALL(generateMode, enter());
  EXPECT_CALL(generateMode, execute(A<Latch const &>(), A<float>()));

  stageEngine.process(0.F);
}

class StageEngineDeferMode : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();
    givenDefer(true);
    stageEngine.process(0.F);
  }
};

TEST_F(StageEngineDeferMode, executesDeferMode_regardlessOfGate_ifDeferIsHigh) {
  givenDefer(true);

  givenGate(false);
  EXPECT_CALL(deferMode, execute());
  stageEngine.process(0.F);

  givenGate(true);
  EXPECT_CALL(deferMode, execute());
  stageEngine.process(0.F);
}

TEST_F(StageEngineDeferMode, beginsTrackingInput_ifGateIsLowWhenDeferFalls) {
  givenDefer(false);
  givenGate(false);

  EXPECT_CALL(deferMode, exit());
  EXPECT_CALL(inputMode, enter());
  EXPECT_CALL(inputMode, execute());

  stageEngine.process(0.F);
}

TEST_F(StageEngineDeferMode, beginsGenerating_ifGateIsHighWhenDeferFalls) {
  givenDefer(false);
  givenGate(true);

  EXPECT_CALL(deferMode, exit());
  EXPECT_CALL(generateMode, enter());
  EXPECT_CALL(generateMode, execute(A<Latch const &>(), A<float>()));

  stageEngine.process(0.F);
}

class StageEngineGenerateMode : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();
    ON_CALL(generateMode, execute(A<Latch const &>(), A<float>())).WillByDefault(Return(Event::Generated));
    givenGate(true);
    stageEngine.process(0.F);
    givenGate(false);
    stageEngine.process(0.F);
  }
};

TEST_F(StageEngineGenerateMode, beginsDeferring_ifDeferRises) {
  givenDefer(true);

  EXPECT_CALL(generateMode, exit());
  EXPECT_CALL(deferMode, enter());
  EXPECT_CALL(deferMode, execute());

  stageEngine.process(0.F);
}

TEST_F(StageEngineGenerateMode, executesGenerateMode_ifDeferIsLowAndGateDoesNotRise) {
  auto constexpr sampleTime{0.04534F};

  givenDefer(false);
  givenGate(false);

  EXPECT_CALL(generateMode, execute(A<Latch const &>(), sampleTime));

  stageEngine.process(sampleTime);
}

TEST_F(StageEngineGenerateMode, passesGateStateToGenerateMode_ifDeferIsLowAndGateDoesNotRise) {
  givenDefer(false);
  givenGate(false);
  stageEngine.process(0.F); // Set latch low

  givenGate(false); // Low with no edge
  EXPECT_CALL(generateMode, execute(Latch{false, false}, A<float>()));
  stageEngine.process(0.F);

  givenGate(true); // Rise
  EXPECT_CALL(generateMode, execute(Latch{true, true}, A<float>()));
  stageEngine.process(0.F);

  givenGate(true); // High with no edge
  EXPECT_CALL(generateMode, execute(Latch{true, false}, A<float>()));
  stageEngine.process(0.F);

  givenGate(false); // Fall
  EXPECT_CALL(generateMode, execute(Latch{false, true}, A<float>()));
  stageEngine.process(0.F);
}

TEST_F(StageEngineGenerateMode, raisesEoc_ifDoneGenerating) {
  ON_CALL(generateMode, execute(A<Latch const &>(), A<float>())).WillByDefault(Return(Event::Completed));

  EXPECT_CALL(controls, showEoc(true));

  stageEngine.process(0.F);
}

class StageEngineLevelMode : public StageEngineTest {
  void SetUp() override {
    StageEngineTest::SetUp();

    // Start generating
    givenGate(true);
    ON_CALL(generateMode, execute(A<Latch const &>(), A<float>())).WillByDefault(Return(Event::Generated));
    stageEngine.process(0.F);

    // Finish generating, which enters TrackingLevel mode
    givenGate(false);
    ON_CALL(generateMode, execute(A<Latch const &>(), A<float>())).WillByDefault(Return(Event::Completed));
    stageEngine.process(0.F);
  }
};

TEST_F(StageEngineLevelMode, beginsDeferring_ifDeferRises) {
  givenDefer(true);

  EXPECT_CALL(levelMode, exit());
  EXPECT_CALL(deferMode, enter());
  EXPECT_CALL(deferMode, execute());

  stageEngine.process(0.F);
}

TEST_F(StageEngineLevelMode, executesLevelMode_ifGateDoesNotRiseAndDeferIsLow) {
  givenDefer(false);
  givenGate(false);

  EXPECT_CALL(levelMode, execute());

  stageEngine.process(0.F);
}

TEST_F(StageEngineLevelMode, beginsGenerating_ifGateRisesAndDeferIsLow) {
  givenDefer(false);
  givenGate(true);

  EXPECT_CALL(levelMode, exit());
  EXPECT_CALL(generateMode, enter());
  EXPECT_CALL(generateMode, execute(A<Latch const &>(), A<float>()));

  stageEngine.process(0.F);
}
