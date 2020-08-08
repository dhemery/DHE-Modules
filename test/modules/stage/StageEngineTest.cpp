#include "modules/stage/StageEngine.h"

#include "components/Latch.h"
#include "fake/FakeControls.h"
#include "fake/FakeModes.h"
#include "modules/stage/Event.h"
#include "modules/stage/Mode.h"

#include <doctest.h>

namespace test {
namespace stage {
  namespace hostage_engine {
    using dhe::Latch;
    using Event = dhe::stage::Event;
    using Mode = dhe::stage::Mode;
    using StageEngine
        = dhe::stage::StageEngine<FakeControls, FakeSimpleMode, FakeSimpleMode, FakeTimedMode, FakeSimpleMode>;

    TEST_CASE("stage::StageEngine") {
      FakeControls controls{};
      FakeSimpleMode deferMode{};
      FakeTimedMode generateMode{};
      FakeSimpleMode inputMode{};
      FakeSimpleMode levelMode{};

      StageEngine engine{controls, deferMode, inputMode, generateMode, levelMode};

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

          SUBCASE("begins generating if gate rises") {
            controls.gate = []() -> bool { return true; };

            engine.process(0.F);

            CHECK(inputMode.wasExited());
            CHECK_FALSE(inputMode.wasExecuted());
            CHECK_FALSE(inputMode.isActive());
            CHECK(generateMode.wasEntered());
            CHECK(generateMode.wasExecuted());
            CHECK(generateMode.isActive());
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

          SUBCASE("begins generating if gate is high") {
            controls.gate = []() -> bool { return true; };

            engine.process(0.F);

            CHECK(deferMode.wasExited());
            CHECK_FALSE(deferMode.wasExecuted());
            CHECK_FALSE(deferMode.isActive());
            CHECK(generateMode.wasEntered());
            CHECK(generateMode.wasExecuted());
            CHECK(generateMode.isActive());
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
      }

      SUBCASE("in generate mode") {
        controls.defer = []() -> bool { return false; };
        controls.gate = []() -> bool { return true; };
        generateMode.returns(Event::Generated);
        engine.process(0.F);
        controls.gate = []() -> bool { return false; };
        generateMode.reset();

        SUBCASE("with defer low") {
          controls.defer = []() -> bool { return false; };

          SUBCASE("executes regardless of gate") {
            controls.gate = []() -> bool { return false; };
            engine.process(1.F);
            CHECK(generateMode.wasExecuted());
            generateMode.reset();

            controls.gate = []() -> bool { return true; };
            engine.process(1.F);
            CHECK(generateMode.wasExecuted());
          }

          SUBCASE("passes gate state to execute") {
            // Set latch low
            controls.gate = []() -> bool { return false; };
            engine.process(0.F);

            controls.gate = []() -> bool { return false; }; // Low with no edge
            engine.process(0.F);
            CHECK_EQ(generateMode.latch(), Latch{false, false});

            controls.gate = []() -> bool { return true; }; // Rise
            engine.process(0.F);
            CHECK_EQ(generateMode.latch(), Latch{true, true});

            controls.gate = []() -> bool { return true; }; // High with no edge
            engine.process(0.F);
            CHECK_EQ(generateMode.latch(), Latch{true, false});

            controls.gate = []() -> bool { return false; }; // Fall
            engine.process(0.F);
            CHECK_EQ(generateMode.latch(), Latch{false, true});
          }

          SUBCASE("if stage completes") {
            generateMode.returns(Event::Completed);
            levelMode.returns(Event::Generated);
            levelMode.reset();

            SUBCASE("begins tracking level") {
              engine.process(0.F);

              CHECK(generateMode.wasExited());
              CHECK_FALSE(generateMode.isActive());
              CHECK(levelMode.wasEntered());
              CHECK(levelMode.isActive());
            }

            SUBCASE("raises eoc") {
              bool raisedEoc{false};
              controls.showEoc = [&](bool e) { raisedEoc = e; };

              engine.process(0.F);

              CHECK(raisedEoc);
            }
          }
        }

        SUBCASE("begins deferring if defer rises") {
          controls.defer = []() -> bool { return true; };

          engine.process(0.F);

          CHECK(generateMode.wasExited());
          CHECK_FALSE(generateMode.wasExecuted());
          CHECK_FALSE(generateMode.isActive());
          CHECK(deferMode.wasEntered());
          CHECK(deferMode.wasExecuted());
          CHECK(deferMode.isActive());
        }
      }

      SUBCASE("in level mode") {
        // Start generating
        controls.defer = []() -> bool { return false; };
        controls.gate = []() -> bool { return true; };
        generateMode.returns(Event::Generated);
        engine.process(0.F);

        // Finish generating, which enters Level mode
        controls.gate = []() -> bool { return false; };
        generateMode.returns(Event::Completed);
        engine.process(0.F);
        generateMode.reset();
        levelMode.reset();

        SUBCASE("with defer low") {
          controls.defer = []() -> bool { return false; };

          SUBCASE("executes if gate does not rise") {
            controls.gate = []() -> bool { return false; };

            engine.process(1.F);

            CHECK(levelMode.wasExecuted());
          }

          SUBCASE("if gate rises") {
            controls.gate = []() -> bool { return true; };

            SUBCASE("begins generating") {
              generateMode.returns(Event::Generated);

              engine.process(0.F);

              CHECK(levelMode.wasExited());
              CHECK_FALSE(levelMode.wasExecuted());
              CHECK_FALSE(levelMode.isActive());
              CHECK(generateMode.wasEntered());
              CHECK(generateMode.wasExecuted());
              CHECK(generateMode.isActive());
            }
          }
        }

        SUBCASE("begins deferring if defer rises") {
          controls.defer = []() -> bool { return true; };
          deferMode.returns(Event::Generated);

          engine.process(0.F);

          CHECK(levelMode.wasExited());
          CHECK_FALSE(levelMode.wasExecuted());
          CHECK_FALSE(levelMode.isActive());
          CHECK(deferMode.wasEntered());
          CHECK(deferMode.wasExecuted());
          CHECK(deferMode.isActive());
        }
      }
    }
  } // namespace hostage_engine
} // namespace stage
} // namespace test
