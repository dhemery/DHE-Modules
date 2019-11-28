#include "stage/HostageEngine.h"

#include "components/Latch.h"
#include "stage/Event.h"
#include "stage/Mode.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using Event = dhe::stage::Event;
using Mode = dhe::stage::Mode;
using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class HostageEngineTest : public Test {
  class Controls {
  public:
    MOCK_METHOD(bool, defer, (), (const));
    MOCK_METHOD(float, duration, (), (const));
    MOCK_METHOD(Mode, mode, (), (const));
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

  class HoldMode {
  public:
    MOCK_METHOD(void, enter, (), ());
    MOCK_METHOD(dhe::stage::Event, execute, (Latch const &, float), ());
    MOCK_METHOD(void, exit, (), ());
  };

  class SustainMode {
  public:
    MOCK_METHOD(void, enter, (), ());
    MOCK_METHOD(dhe::stage::Event, execute, (Latch const &), ());
    MOCK_METHOD(void, exit, (), ());
  };

protected:
  static auto constexpr defaultDuration{1.F};
  static auto constexpr defaultModeSelection{Mode::Hold};
  NiceMock<Controls> controls{};
  NiceMock<EventlessMode> deferMode{};
  NiceMock<HoldMode> holdMode{};
  NiceMock<SustainMode> sustainMode{};
  NiceMock<EventlessMode> idleMode{};
  NiceMock<EventlessMode> inputMode{};
  dhe::stage::HostageEngine<Controls, EventlessMode, EventlessMode, HoldMode, SustainMode, EventlessMode> engine{
      controls, inputMode, deferMode, holdMode, sustainMode, idleMode};

  void givenDefer(bool defer) { ON_CALL(controls, defer()).WillByDefault(Return(defer)); }
  void givenDuration(float duration) { ON_CALL(controls, duration()).WillByDefault(Return(duration)); }
  void givenGate(bool gate) { ON_CALL(controls, gate()).WillByDefault(Return(gate)); }
  void givenModeSelection(Mode mode) { ON_CALL(controls, mode()).WillByDefault(Return(mode)); }

  void SetUp() override {
    givenDuration(defaultDuration);
    givenModeSelection(defaultModeSelection);
  }
};

TEST_F(HostageEngineTest, startsInInputMode) {
  EXPECT_CALL(inputMode, execute());

  engine.process(0.F);
}

class HostageEngineDeferMode : public HostageEngineTest {
  void SetUp() override {
    HostageEngineTest::SetUp();
    givenDefer(true);
    engine.process(0.F);
  }
};

TEST_F(HostageEngineDeferMode, executesDeferMode_regardlessOfGate_ifDeferIsHigh) {
  givenDefer(true);

  givenGate(false);
  EXPECT_CALL(deferMode, execute());
  engine.process(0.F);

  givenGate(true);
  EXPECT_CALL(deferMode, execute());
  engine.process(0.F);
}

TEST_F(HostageEngineDeferMode, beginsTrackingInput_ifDeferFalls) {
  givenDefer(false);

  EXPECT_CALL(deferMode, exit());
  EXPECT_CALL(inputMode, enter());
  EXPECT_CALL(inputMode, execute());

  engine.process(0.F);
}

TEST_F(HostageEngineDeferMode, withHoldModeSelected_beginsHolding_ifGateIsHighWhenDeferFalls) {
  givenDefer(false);
  givenGate(true);
  givenModeSelection(Mode::Hold);

  EXPECT_CALL(deferMode, exit());
  EXPECT_CALL(holdMode, enter());
  EXPECT_CALL(holdMode, execute(A<Latch const &>(), A<float>()));

  engine.process(0.F);
}

TEST_F(HostageEngineDeferMode, withHoldModeSelected_beginsTrackingInput_ifGateIsLowWhenDeferFalls) {
  givenDefer(false);
  givenGate(false);
  givenModeSelection(Mode::Hold);

  EXPECT_CALL(deferMode, exit());
  EXPECT_CALL(inputMode, enter());
  EXPECT_CALL(inputMode, execute());

  engine.process(0.F);
}

TEST_F(HostageEngineDeferMode, withHoldModeSelected_raisesEoc_ifGateIsLowWhenDeferFalls) {
  givenDefer(false);
  givenGate(false);
  givenModeSelection(Mode::Hold);

  EXPECT_CALL(controls, showEoc(true));

  engine.process(0.F);
}

TEST_F(HostageEngineDeferMode, withSustainModeSelected_beginsSustaining_ifGateIsHighWhenDeferFalls) {
  givenDefer(false);
  givenGate(true);
  givenModeSelection(Mode::Sustain);

  EXPECT_CALL(deferMode, exit());
  EXPECT_CALL(sustainMode, enter());
  EXPECT_CALL(sustainMode, execute(A<Latch const &>()));

  engine.process(0.F);
}

TEST_F(HostageEngineDeferMode, withSustainModeSelected_beginsTrackingInput_ifGateIsLowWhenDeferFalls) {
  givenDefer(false);
  givenGate(false);
  givenModeSelection(Mode::Sustain);

  EXPECT_CALL(deferMode, exit());
  EXPECT_CALL(inputMode, enter());
  EXPECT_CALL(inputMode, execute());

  engine.process(0.F);
}

TEST_F(HostageEngineDeferMode, withSustainModeSelected_raisesEoc_ifGateIsLowWhenDeferFalls) {
  givenDefer(false);
  givenGate(false);
  givenModeSelection(Mode::Sustain);

  EXPECT_CALL(controls, showEoc(true));

  engine.process(0.F);
}

class HostageEngineHoldMode : public HostageEngineTest {
  void SetUp() override {
    HostageEngineTest::SetUp();
    givenModeSelection(Mode::Hold);
    ON_CALL(holdMode, execute(A<Latch const &>(), A<float>())).WillByDefault(Return(Event::Generated));
    givenGate(true);
    engine.process(0.F);
  }
};

TEST_F(HostageEngineHoldMode, beginsDeferring_ifDeferRises) {
  givenDefer(true);

  EXPECT_CALL(holdMode, exit());
  EXPECT_CALL(deferMode, enter());
  EXPECT_CALL(deferMode, execute());

  engine.process(0.F);
}

TEST_F(HostageEngineHoldMode, executesHoldMode_ifDeferIsLow_regardlessOfGate) {
  givenDefer(false);

  auto constexpr sampleTime{0.04534F};

  givenGate(false);
  EXPECT_CALL(holdMode, execute(A<Latch const &>(), sampleTime));
  engine.process(sampleTime);

  givenGate(true);
  EXPECT_CALL(holdMode, execute(A<Latch const &>(), sampleTime));
  engine.process(sampleTime);
}

TEST_F(HostageEngineHoldMode, passesGateStateWhenExecutingGenerateMode) {
  givenDefer(false);
  givenGate(false);
  engine.process(0.F); // Set latch low

  givenGate(false); // Low with no edge
  EXPECT_CALL(holdMode, execute(Latch{false, false}, A<float>()));
  engine.process(0.F);

  givenGate(true); // Rise
  EXPECT_CALL(holdMode, execute(Latch{true, true}, A<float>()));
  engine.process(0.F);

  givenGate(true); // High with no edge
  EXPECT_CALL(holdMode, execute(Latch{true, false}, A<float>()));
  engine.process(0.F);

  givenGate(false); // Fall
  EXPECT_CALL(holdMode, execute(Latch{false, true}, A<float>()));
  engine.process(0.F);
}

TEST_F(HostageEngineHoldMode, raisesEoc_ifHoldModeCompletes) {
  ON_CALL(holdMode, execute(A<Latch const &>(), A<float>())).WillByDefault(Return(Event::Completed));

  EXPECT_CALL(controls, showEoc(true));

  engine.process(0.F);
}

class HostageEngineIdleMode : public HostageEngineTest {
  void SetUp() override {
    HostageEngineTest::SetUp();
    givenModeSelection(Mode::Sustain);

    // Start sustaining
    givenGate(true);
    ON_CALL(sustainMode, execute(A<Latch const &>())).WillByDefault(Return(Event::Generated));
    engine.process(0.F);

    // Finish sustaining, which enters Idle mode
    givenGate(false);
    ON_CALL(sustainMode, execute(A<Latch const &>())).WillByDefault(Return(Event::Completed));
    engine.process(0.F);
  }
};

TEST_F(HostageEngineIdleMode, beginsDeferring_ifDeferRises) {
  givenDefer(true);

  EXPECT_CALL(idleMode, exit());
  EXPECT_CALL(deferMode, enter());
  EXPECT_CALL(deferMode, execute());

  engine.process(0.F);
}

TEST_F(HostageEngineIdleMode, executesIdleMode_ifGateDoesNotRiseAndDeferIsLow) {
  givenDefer(false);
  givenGate(false);

  EXPECT_CALL(idleMode, execute());

  engine.process(0.F);
}

TEST_F(HostageEngineIdleMode, withHoldModeSelected_beginsHolding_ifGateRisesAndDeferIsLow) {
  givenDefer(false);
  givenGate(true);
  givenModeSelection(Mode::Hold);

  EXPECT_CALL(idleMode, exit());
  EXPECT_CALL(holdMode, enter());
  EXPECT_CALL(holdMode, execute(A<Latch const &>(), A<float>()));

  engine.process(0.F);
}

TEST_F(HostageEngineIdleMode, withSustainModeSelected_beginsSustaining_ifGateRisesAndDeferIsLow) {
  givenDefer(false);
  givenGate(true);
  givenModeSelection(Mode::Sustain);

  EXPECT_CALL(idleMode, exit());
  EXPECT_CALL(sustainMode, enter());
  EXPECT_CALL(sustainMode, execute(A<Latch const &>()));

  engine.process(0.F);
}

class HostageEngineInputMode : public HostageEngineTest {
  void SetUp() override {
    HostageEngineTest::SetUp();
    engine.process(0.F);
  }
};

TEST_F(HostageEngineInputMode, beginsDeferring_ifDeferRises) {
  givenDefer(true);

  EXPECT_CALL(inputMode, exit());
  EXPECT_CALL(deferMode, enter());
  EXPECT_CALL(deferMode, execute());

  engine.process(0.F);
}

TEST_F(HostageEngineInputMode, executesInputMode_ifGateDoesNotRiseAndDeferIsLow) {
  givenDefer(false);
  givenGate(false);

  EXPECT_CALL(inputMode, execute());

  engine.process(0.F);
}

TEST_F(HostageEngineInputMode, withHoldModeSeleced_beginsHolding_ifGateRisesAndfDeferIsLow) {
  givenDefer(false);
  givenGate(true);
  givenModeSelection(Mode::Hold);

  EXPECT_CALL(inputMode, exit());
  EXPECT_CALL(holdMode, enter());
  EXPECT_CALL(holdMode, execute(A<Latch const &>(), A<float>()));

  engine.process(0.F);
}

TEST_F(HostageEngineInputMode, withSustainModeSelected_beginsSustaining_ifGateRisesAndfDeferIsLow) {
  givenDefer(false);
  givenGate(true);
  givenModeSelection(Mode::Sustain);

  EXPECT_CALL(inputMode, exit());
  EXPECT_CALL(sustainMode, enter());
  EXPECT_CALL(sustainMode, execute(A<Latch const &>()));

  engine.process(0.F);
}

class HostageEngineSustainMode : public HostageEngineTest {
  void SetUp() override {
    HostageEngineTest::SetUp();
    givenModeSelection(Mode::Sustain);
    ON_CALL(sustainMode, execute(A<Latch const &>())).WillByDefault(Return(Event::Generated));
    givenGate(true);
    engine.process(0.F);
  }
};

TEST_F(HostageEngineSustainMode, beginsDeferring_ifDeferRises) {
  givenDefer(true);

  EXPECT_CALL(sustainMode, exit());
  EXPECT_CALL(deferMode, enter());
  EXPECT_CALL(deferMode, execute());

  engine.process(0.F);
}

TEST_F(HostageEngineSustainMode, executesSustainMode_ifDeferIsLow_regardlessOfGate) {
  givenDefer(false);

  givenGate(false);
  EXPECT_CALL(sustainMode, execute(A<Latch const &>()));
  engine.process(0.F);

  givenGate(true);
  EXPECT_CALL(sustainMode, execute(A<Latch const &>()));
  engine.process(0.F);
}

TEST_F(HostageEngineSustainMode, passesGateStateWhenExecutingSustainMode) {
  givenDefer(false);
  givenGate(false);
  engine.process(0.F); // Set latch low

  givenGate(false); // Low with no edge
  EXPECT_CALL(sustainMode, execute(Latch{false, false}));
  engine.process(0.F);

  givenGate(true); // Rise
  EXPECT_CALL(sustainMode, execute(Latch{true, true}));
  engine.process(0.F);

  givenGate(true); // High with no edge
  EXPECT_CALL(sustainMode, execute(Latch{true, false}));
  engine.process(0.F);

  givenGate(false); // Fall
  EXPECT_CALL(sustainMode, execute(Latch{false, true}));
  engine.process(0.F);
}

TEST_F(HostageEngineSustainMode, raisesEoc_ifSustainModeCompletes) {
  ON_CALL(sustainMode, execute(A<Latch const &>())).WillByDefault(Return(Event::Completed));

  EXPECT_CALL(controls, showEoc(true));

  engine.process(0.F);
}
