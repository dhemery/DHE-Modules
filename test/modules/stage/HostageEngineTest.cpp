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
    using HostageEngine = dhe::stage::HostageEngine<FakeControls, FakeSimpleMode, FakeSimpleMode, FakeTimedMode,
                                                    FakeLatchedMode, FakeSimpleMode>;

    TEST_CASE("stage::HostageEngine") {
      FakeControls controls{};
      FakeSimpleMode deferMode{};
      FakeTimedMode holdMode{};
      FakeSimpleMode idleMode{};
      FakeSimpleMode inputMode{};
      FakeLatchedMode sustainMode{};

      HostageEngine engine{controls, inputMode, deferMode, holdMode, sustainMode, idleMode};

      controls.showEoc = [](bool e) {};

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

      SUBCASE("in defer mode") {
        controls.defer = []() -> bool { return true; };
        controls.gate = []() -> bool { return false; };
        engine.process(0.F);
        deferMode.reset();

        SUBCASE("with defer high") {
          controls.defer = []() -> bool { return true; };

          SUBCASE("executes defer mode regardless of gate") {
            controls.gate = []() -> bool { return true; };

            engine.process(0.F);
            CHECK(deferMode.wasExecuted());
            deferMode.reset();

            controls.gate = []() -> bool { return false; };
            engine.process(0.F);
            CHECK(deferMode.wasExecuted());
          }
        }

        SUBCASE("if defer falls") {
          controls.defer = []() -> bool { return false; };

          SUBCASE("with hold mode selected") {
            controls.mode = []() -> Mode { return Mode::Hold; };

            SUBCASE("begins holding if gate is high") {
              controls.gate = []() -> bool { return true; };

              engine.process(0.F);

              CHECK(deferMode.wasExited());
              CHECK_FALSE(deferMode.wasExecuted());
              CHECK_FALSE(deferMode.isActive());
              CHECK(holdMode.wasEntered());
              CHECK(holdMode.wasExecuted());
              CHECK(holdMode.isActive());
            }

            SUBCASE("if gate is low") {
              controls.gate = []() -> bool { return false; };

              SUBCASE("begins tracking input") {
                engine.process(0.F);

                CHECK(deferMode.wasExited());
                CHECK_FALSE(deferMode.wasExecuted());
                CHECK_FALSE(deferMode.isActive());
                CHECK(inputMode.wasEntered());
                CHECK(inputMode.wasExecuted());
                CHECK(inputMode.isActive());
              }

              SUBCASE("does not raise eoc") {
                bool eoc{false};
                controls.showEoc = [&](bool e) { eoc = e; };

                engine.process(0.F);
                CHECK_FALSE(eoc);
              }
            }
          }

          SUBCASE("with sustain mode selected") {
            controls.mode = []() -> Mode { return Mode::Sustain; };

            SUBCASE("begins sustaining if gate is high") {
              controls.gate = []() -> bool { return true; };

              engine.process(0.F);

              CHECK(deferMode.wasExited());
              CHECK_FALSE(deferMode.wasExecuted());
              CHECK_FALSE(deferMode.isActive());
              CHECK(sustainMode.wasEntered());
              CHECK(sustainMode.wasExecuted());
              CHECK(sustainMode.isActive());
            }

            SUBCASE("if gate is low") {
              controls.gate = []() -> bool { return false; };

              SUBCASE("becomes idle") {
                engine.process(0.F);

                CHECK(deferMode.wasExited());
                CHECK_FALSE(deferMode.wasExecuted());
                CHECK_FALSE(deferMode.isActive());
                CHECK(idleMode.wasEntered());
                CHECK(idleMode.wasExecuted());
                CHECK(idleMode.isActive());
              }

              SUBCASE("raises eoc") {
                bool eoc{false};
                controls.showEoc = [&](bool e) { eoc = e; };

                engine.process(0.F);
                CHECK(eoc);
              }
            }
          }
        }
      }
    }

    /*

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
