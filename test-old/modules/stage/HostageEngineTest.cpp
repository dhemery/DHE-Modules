#include "modules/stage/hostage-engine.h"

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
using HostageEngine =
    dhe::stage::HostageEngine<FakeControls, FakeSimpleMode, FakeSimpleMode,
                              FakeTimedMode, FakeLatchedMode, FakeSimpleMode>;

TEST_CASE("stage::HostageEngine") {
  FakeControls controls{};
  FakeSimpleMode defer_mode{};
  FakeTimedMode hold_mode{};
  FakeSimpleMode idle_mode{};
  FakeSimpleMode input_mode{};
  FakeLatchedMode sustain_mode{};

  HostageEngine engine{controls,  input_mode,   defer_mode,
                       hold_mode, sustain_mode, idle_mode};

  controls.show_eoc = [](bool e) {};

  SUBCASE("in hold mode") {
    controls.defer = []() -> bool { return false; };
    controls.gate = []() -> bool { return true; };
    controls.mode = []() -> Mode { return Mode::Hold; };
    hold_mode.returns(Event::Generated);
    engine.process(0.F);
    controls.gate = []() -> bool { return false; };
    hold_mode.reset();

    SUBCASE("with defer low") {
      controls.defer = []() -> bool { return false; };

      SUBCASE("executes regardless of gate") {
        controls.gate = []() -> bool { return false; };
        engine.process(1.F);
        CHECK(hold_mode.wasExecuted());
        hold_mode.reset();

        controls.gate = []() -> bool { return true; };
        engine.process(1.F);
        CHECK(hold_mode.wasExecuted());
      }

      SUBCASE("passes gate state to execute") {
        // Set latch low
        controls.gate = []() -> bool { return false; };
        engine.process(0.F);

        controls.gate = []() -> bool { return false; }; // Low with no edge
        engine.process(0.F);
        CHECK_EQ(hold_mode.latch(), Latch{false, false});

        controls.gate = []() -> bool { return true; }; // Rise
        engine.process(0.F);
        CHECK_EQ(hold_mode.latch(), Latch{true, true});

        controls.gate = []() -> bool { return true; }; // High with no edge
        engine.process(0.F);
        CHECK_EQ(hold_mode.latch(), Latch{true, false});

        controls.gate = []() -> bool { return false; }; // Fall
        engine.process(0.F);
        CHECK_EQ(hold_mode.latch(), Latch{false, true});
      }

      SUBCASE("raises eoc if hold completes") {
        hold_mode.returns(Event::Completed);

        bool raised_eoc{false};
        controls.show_eoc = [&raised_eoc](bool e) { raised_eoc = e; };

        engine.process(0.F);

        CHECK(raised_eoc);
      }
    }

    SUBCASE("begins deferring if defer rises") {
      controls.defer = []() -> bool { return true; };

      engine.process(0.F);

      CHECK(hold_mode.wasExited());
      CHECK_FALSE(hold_mode.wasExecuted());
      CHECK_FALSE(hold_mode.isActive());
      CHECK(defer_mode.wasEntered());
      CHECK(defer_mode.wasExecuted());
      CHECK(defer_mode.isActive());
    }
  }

  SUBCASE("in sustain mode") {
    controls.defer = []() -> bool { return false; };
    controls.gate = []() -> bool { return true; };
    controls.mode = []() -> Mode { return Mode::Sustain; };
    sustain_mode.returns(Event::Generated);
    engine.process(0.F);
    sustain_mode.reset();

    SUBCASE("with defer low") {
      controls.defer = []() -> bool { return false; };

      SUBCASE("executes regardless of gate") {
        controls.gate = []() -> bool { return false; };
        engine.process(1.F);
        CHECK(sustain_mode.wasExecuted());
        hold_mode.reset();

        controls.gate = []() -> bool { return true; };
        engine.process(1.F);
        CHECK(sustain_mode.wasExecuted());
      }

      SUBCASE("passes gate state to execute") {
        // Set latch low
        controls.gate = []() -> bool { return false; };
        engine.process(0.F);

        controls.gate = []() -> bool { return false; }; // Low with no edge
        engine.process(0.F);
        CHECK_EQ(sustain_mode.latch(), Latch{false, false});

        controls.gate = []() -> bool { return true; }; // Rise
        engine.process(0.F);
        CHECK_EQ(sustain_mode.latch(), Latch{true, true});

        controls.gate = []() -> bool { return true; }; // High with no edge
        engine.process(0.F);
        CHECK_EQ(sustain_mode.latch(), Latch{true, false});

        controls.gate = []() -> bool { return false; }; // Fall
        engine.process(0.F);
        CHECK_EQ(sustain_mode.latch(), Latch{false, true});
      }

      SUBCASE("raises eoc if sustain completes") {
        sustain_mode.returns(Event::Completed);

        bool raised_eoc{false};
        controls.show_eoc = [&raised_eoc](bool e) { raised_eoc = e; };

        engine.process(0.F);

        CHECK(raised_eoc);
      }
    }

    SUBCASE("begins deferring if defer rises") {
      controls.defer = []() -> bool { return true; };
      defer_mode.returns(Event::Generated);

      engine.process(0.F);

      CHECK(sustain_mode.wasExited());
      CHECK_FALSE(sustain_mode.wasExecuted());
      CHECK_FALSE(sustain_mode.isActive());
      CHECK(defer_mode.wasEntered());
      CHECK(defer_mode.wasExecuted());
      CHECK(defer_mode.isActive());
    }
  }
}
} // namespace hostage_engine
} // namespace stage
} // namespace test
