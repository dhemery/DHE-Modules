#include "modules/stage/HostageEngine.h"

#include "components/Latch.h"
#include "doctest/doctest.h"
#include "fake/FakeControls.h"
#include "fake/FakeModes.h"
#include "modules/stage/Event.h"
#include "modules/stage/Mode.h"

namespace test {
namespace stage {
  namespace hostage_engine {
    using dhe::Latch;
    using Event = dhe::stage::Event;
    using Mode = dhe::stage::Mode;
    using HostageEngine = dhe::stage::HostageEngine<FakeControls, SimpleFakeMode, SimpleFakeMode, TimedFakeMode,
                                                    LatchedFakeMode, SimpleFakeMode>;

    TEST_CASE("stage::HostageEngine") {
      FakeControls controls{};
      SimpleFakeMode deferMode{};
      TimedFakeMode holdMode{};
      SimpleFakeMode idleMode{};
      SimpleFakeMode inputMode{};
      LatchedFakeMode sustainMode{};

      HostageEngine engine{controls, inputMode, deferMode, holdMode, sustainMode, idleMode};

      SUBCASE("starts in input mode") {
        controls.defer = []() -> bool { return false; };
        controls.gate = []() -> bool { return false; };
        controls.showEoc = [](bool e) {};

        inputMode.returns(Event::Generated);

        engine.process(0.F);

        CHECK(inputMode.wasExecuted());
      }

      SUBCASE("in input mode") {
        controls.defer = []() -> bool { return false; };
        controls.gate = []() -> bool { return false; };
        controls.showEoc = [](bool e) {};

        inputMode.returns(Event::Generated);
        engine.process(0.F);
        inputMode.reset();

        SUBCASE("with defer low") {
          controls.defer = []() -> bool { return false; };

          SUBCASE("executes if gate does not rise") {
            controls.gate = []() -> bool { return false; };

            engine.process(1.F);

            CHECK(inputMode.wasExecuted());
          }

          SUBCASE("if gate rises") {
            controls.gate = []() -> bool { return true; };

            SUBCASE("begins holding if hold mode selected") {
              controls.mode = []() -> Mode { return Mode::Hold; };

              engine.process(0.F);

              CHECK(inputMode.wasExited());
              CHECK_FALSE(inputMode.wasExecuted());
              CHECK_FALSE(inputMode.isActive());
              CHECK(holdMode.wasEntered());
              CHECK(holdMode.wasExecuted());
              CHECK(holdMode.isActive());
            }

            SUBCASE("begins sustaining if sustain mode selected") {
              controls.mode = []() -> Mode { return Mode::Sustain; };

              engine.process(0.F);

                CHECK(inputMode.wasExited());
                CHECK_FALSE(inputMode.wasExecuted());
                CHECK_FALSE(inputMode.isActive());
                CHECK(sustainMode.wasEntered());
                CHECK(sustainMode.wasExecuted());
                CHECK(sustainMode.isActive());
            }
          }
        }

        SUBCASE("begins deferring if defer rises") {
          controls.defer = []() -> bool { return true; };
          deferMode.returns(Event::Generated);

          engine.process(0.F);

          CHECK(inputMode.wasExited());
          CHECK_FALSE(inputMode.wasExecuted());
          CHECK_FALSE(inputMode.isActive());
          CHECK(deferMode.wasEntered());
          CHECK(deferMode.wasExecuted());
          CHECK(deferMode.isActive());
        }
      }
    }

    /*

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

    TEST_F(HostageEngineDeferMode, withHoldModeSelected_doesNotRaiseEoc_ifGateIsLowWhenDeferFalls) {
      givenDefer(false);
      givenGate(false);
      givenModeSelection(Mode::Hold);

      EXPECT_CALL(controls, showEoc(false));

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

    TEST_F(HostageEngineDeferMode, withSustainModeSelected_becomesIdle_ifGateIsLowWhenDeferFalls) {
      givenDefer(false);
      givenGate(false);
      givenModeSelection(Mode::Sustain);

      EXPECT_CALL(deferMode, exit());
      EXPECT_CALL(idleMode, enter());
      EXPECT_CALL(idleMode, execute());

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
    }*/
  } // namespace hostage_engine
} // namespace stage
} // namespace test
