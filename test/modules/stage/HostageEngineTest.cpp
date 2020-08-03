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
                bool raisedEoc{false};
                controls.showEoc = [&](bool e) { raisedEoc = e; };

                engine.process(0.F);
                CHECK_FALSE(raisedEoc);
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
                bool raisedEoc{false};
                controls.showEoc = [&](bool e) { raisedEoc = e; };

                engine.process(0.F);
                CHECK(raisedEoc);
              }
            }
          }
        }
      }

      SUBCASE("in hold mode") {
        controls.defer = []() -> bool { return false; };
        controls.gate = []() -> bool { return true; };
        controls.mode = []() -> Mode { return Mode::Hold; };
        holdMode.returns(Event::Generated);
        engine.process(0.F);
        controls.gate = []() -> bool { return false; };
        holdMode.reset();

        SUBCASE("with defer low") {
          controls.defer = []() -> bool { return false; };

          SUBCASE("executes regardless of gate") {
            controls.gate = []() -> bool { return false; };
            engine.process(1.F);
            CHECK(holdMode.wasExecuted());
            holdMode.reset();

            controls.gate = []() -> bool { return true; };
            engine.process(1.F);
            CHECK(holdMode.wasExecuted());
          }

          SUBCASE("passes gate state to execute") {
            // Set latch low
            controls.gate = []() -> bool { return false; };
            engine.process(0.F);

            controls.gate = []() -> bool { return false; }; // Low with no edge
            engine.process(0.F);
            CHECK_EQ(holdMode.latch(), Latch{false, false});

            controls.gate = []() -> bool { return true; }; // Rise
            engine.process(0.F);
            CHECK_EQ(holdMode.latch(), Latch{true, true});

            controls.gate = []() -> bool { return true; }; // High with no edge
            engine.process(0.F);
            CHECK_EQ(holdMode.latch(), Latch{true, false});

            controls.gate = []() -> bool { return false; }; // Fall
            engine.process(0.F);
            CHECK_EQ(holdMode.latch(), Latch{false, true});
          }

          SUBCASE("raises eoc if hold completes") {
            holdMode.returns(Event::Completed);

            bool raisedEoc{false};
            controls.showEoc = [&](bool e) { raisedEoc = e; };

            engine.process(0.F);

            CHECK(raisedEoc);
          }
        }

        SUBCASE("begins deferring if defer rises") {
          controls.defer = []() -> bool { return true; };

          engine.process(0.F);

          CHECK(holdMode.wasExited());
          CHECK_FALSE(holdMode.wasExecuted());
          CHECK_FALSE(holdMode.isActive());
          CHECK(deferMode.wasEntered());
          CHECK(deferMode.wasExecuted());
          CHECK(deferMode.isActive());
        }
      }

      SUBCASE("in sustain mode") {
        controls.defer = []() -> bool { return false; };
        controls.gate = []() -> bool { return true; };
        controls.mode = []() -> Mode { return Mode::Sustain; };
        sustainMode.returns(Event::Generated);
        engine.process(0.F);
        sustainMode.reset();

        SUBCASE("with defer low") {
          controls.defer = []() -> bool { return false; };

          SUBCASE("executes regardless of gate") {
            controls.gate = []() -> bool { return false; };
            engine.process(1.F);
            CHECK(sustainMode.wasExecuted());
            holdMode.reset();

            controls.gate = []() -> bool { return true; };
            engine.process(1.F);
            CHECK(sustainMode.wasExecuted());
          }

          SUBCASE("passes gate state to execute") {
            // Set latch low
            controls.gate = []() -> bool { return false; };
            engine.process(0.F);

            controls.gate = []() -> bool { return false; }; // Low with no edge
            engine.process(0.F);
            CHECK_EQ(sustainMode.latch(), Latch{false, false});

            controls.gate = []() -> bool { return true; }; // Rise
            engine.process(0.F);
            CHECK_EQ(sustainMode.latch(), Latch{true, true});

            controls.gate = []() -> bool { return true; }; // High with no edge
            engine.process(0.F);
            CHECK_EQ(sustainMode.latch(), Latch{true, false});

            controls.gate = []() -> bool { return false; }; // Fall
            engine.process(0.F);
            CHECK_EQ(sustainMode.latch(), Latch{false, true});
          }

          SUBCASE("raises eoc if sustain completes") {
            sustainMode.returns(Event::Completed);

            bool raisedEoc{false};
            controls.showEoc = [&](bool e) { raisedEoc = e; };

            engine.process(0.F);

            CHECK(raisedEoc);
          }
        }

        SUBCASE("begins deferring if defer rises") {
          controls.defer = []() -> bool { return true; };
          deferMode.returns(Event::Generated);

          engine.process(0.F);

          CHECK(sustainMode.wasExited());
          CHECK_FALSE(sustainMode.wasExecuted());
          CHECK_FALSE(sustainMode.isActive());
          CHECK(deferMode.wasEntered());
          CHECK(deferMode.wasExecuted());
          CHECK(deferMode.isActive());
        }
      }

      SUBCASE("in idle mode") {
        // Start sustaining
        controls.defer = []() -> bool { return false; };
        controls.gate = []() -> bool { return true; };
        controls.mode = []() -> Mode { return Mode::Sustain; };
        sustainMode.returns(Event::Generated);
        engine.process(0.F);

        // Finish sustaining, which enters Idle mode
        controls.gate = []() -> bool { return false; };
        sustainMode.returns(Event::Completed);
        engine.process(0.F);
        sustainMode.reset();
        idleMode.reset();

        SUBCASE("with defer low") {
          controls.defer = []() -> bool { return false; };

          SUBCASE("executes if gate does not rise") {
            controls.gate = []() -> bool { return false; };

            engine.process(1.F);

            CHECK(idleMode.wasExecuted());
          }

          SUBCASE("if gate rises") {
            controls.gate = []() -> bool { return true; };

            SUBCASE("begins holding if hold mode selected") {
              controls.mode = []() -> Mode { return Mode::Hold; };
              holdMode.returns(Event::Generated);

              engine.process(0.F);

              CHECK(idleMode.wasExited());
              CHECK_FALSE(idleMode.wasExecuted());
              CHECK_FALSE(idleMode.isActive());
              CHECK(holdMode.wasEntered());
              CHECK(holdMode.wasExecuted());
              CHECK(holdMode.isActive());
            }

            SUBCASE("begins sustaining if sustain mode selected") {
              controls.mode = []() -> Mode { return Mode::Sustain; };
              sustainMode.returns(Event::Generated);

              engine.process(0.F);

              CHECK(idleMode.wasExited());
              CHECK_FALSE(idleMode.wasExecuted());
              CHECK_FALSE(idleMode.isActive());
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

          CHECK(idleMode.wasExited());
          CHECK_FALSE(idleMode.wasExecuted());
          CHECK_FALSE(idleMode.isActive());
          CHECK(deferMode.wasEntered());
          CHECK(deferMode.wasExecuted());
          CHECK(deferMode.isActive());
        }
      }
    }
  } // namespace hostage_engine
} // namespace stage
} // namespace test
