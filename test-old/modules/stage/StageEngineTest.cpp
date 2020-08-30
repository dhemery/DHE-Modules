#include "modules/stage/stage-engine.h"

#include "components/latch.h"
#include "fake/FakeControls.h"
#include "fake/FakeModes.h"
#include "modules/stage/event.h"
#include "modules/stage/mode.h"

#include <doctest.h>

namespace test {
namespace stage {
namespace hostage_engine {
using dhe::Latch;
using Event = dhe::stage::Event;
using Mode = dhe::stage::Mode;
using StageEngine =
    dhe::stage::StageEngine<FakeControls, FakeSimpleMode, FakeSimpleMode,
                            FakeTimedMode, FakeSimpleMode>;

TEST_CASE("stage::StageEngine") {
  FakeControls controls{};
  FakeSimpleMode defer_mode{};
  FakeTimedMode generate_mode{};
  FakeSimpleMode input_mode{};
  FakeSimpleMode level_mode{};

  StageEngine engine{controls, defer_mode, input_mode, generate_mode,
                     level_mode};

  controls.show_eoc = [](bool e) {};

  SUBCASE("in generate mode") {
    controls.defer = []() -> bool { return false; };
    controls.gate = []() -> bool { return true; };
    generate_mode.returns(Event::Generated);
    engine.process(0.F);
    controls.gate = []() -> bool { return false; };
    generate_mode.reset();

    SUBCASE("with defer low") {
      controls.defer = []() -> bool { return false; };

      SUBCASE("executes regardless of gate") {
        controls.gate = []() -> bool { return false; };
        engine.process(1.F);
        CHECK(generate_mode.wasExecuted());
        generate_mode.reset();

        controls.gate = []() -> bool { return true; };
        engine.process(1.F);
        CHECK(generate_mode.wasExecuted());
      }

      SUBCASE("passes gate state to execute") {
        // Set latch low
        controls.gate = []() -> bool { return false; };
        engine.process(0.F);

        controls.gate = []() -> bool { return false; }; // Low with no edge
        engine.process(0.F);
        CHECK_EQ(generate_mode.latch(), Latch{false, false});

        controls.gate = []() -> bool { return true; }; // Rise
        engine.process(0.F);
        CHECK_EQ(generate_mode.latch(), Latch{true, true});

        controls.gate = []() -> bool { return true; }; // High with no edge
        engine.process(0.F);
        CHECK_EQ(generate_mode.latch(), Latch{true, false});

        controls.gate = []() -> bool { return false; }; // Fall
        engine.process(0.F);
        CHECK_EQ(generate_mode.latch(), Latch{false, true});
      }

      SUBCASE("if stage completes") {
        generate_mode.returns(Event::Completed);
        level_mode.returns(Event::Generated);
        level_mode.reset();

        SUBCASE("begins tracking level") {
          engine.process(0.F);

          CHECK(generate_mode.wasExited());
          CHECK_FALSE(generate_mode.isActive());
          CHECK(level_mode.wasEntered());
          CHECK(level_mode.isActive());
        }

        SUBCASE("raises eoc") {
          bool raised_eoc{false};
          controls.show_eoc = [&](bool e) { raised_eoc = e; };

          engine.process(0.F);

          CHECK(raised_eoc);
        }
      }
    }

    SUBCASE("begins deferring if defer rises") {
      controls.defer = []() -> bool { return true; };

      engine.process(0.F);

      CHECK(generate_mode.wasExited());
      CHECK_FALSE(generate_mode.wasExecuted());
      CHECK_FALSE(generate_mode.isActive());
      CHECK(defer_mode.wasEntered());
      CHECK(defer_mode.wasExecuted());
      CHECK(defer_mode.isActive());
    }
  }

  SUBCASE("in level mode") {
    // Start generating
    controls.defer = []() -> bool { return false; };
    controls.gate = []() -> bool { return true; };
    generate_mode.returns(Event::Generated);
    engine.process(0.F);

    // Finish generating, which enters Level mode
    controls.gate = []() -> bool { return false; };
    generate_mode.returns(Event::Completed);
    engine.process(0.F);
    generate_mode.reset();
    level_mode.reset();

    SUBCASE("with defer low") {
      controls.defer = []() -> bool { return false; };

      SUBCASE("executes if gate does not rise") {
        controls.gate = []() -> bool { return false; };

        engine.process(1.F);

        CHECK(level_mode.wasExecuted());
      }

      SUBCASE("if gate rises") {
        controls.gate = []() -> bool { return true; };

        SUBCASE("begins generating") {
          generate_mode.returns(Event::Generated);

          engine.process(0.F);

          CHECK(level_mode.wasExited());
          CHECK_FALSE(level_mode.wasExecuted());
          CHECK_FALSE(level_mode.isActive());
          CHECK(generate_mode.wasEntered());
          CHECK(generate_mode.wasExecuted());
          CHECK(generate_mode.isActive());
        }
      }
    }

    SUBCASE("begins deferring if defer rises") {
      controls.defer = []() -> bool { return true; };
      defer_mode.returns(Event::Generated);

      engine.process(0.F);

      CHECK(level_mode.wasExited());
      CHECK_FALSE(level_mode.wasExecuted());
      CHECK_FALSE(level_mode.isActive());
      CHECK(defer_mode.wasEntered());
      CHECK(defer_mode.wasExecuted());
      CHECK(defer_mode.isActive());
    }
  }
}
} // namespace hostage_engine
} // namespace stage
} // namespace test
