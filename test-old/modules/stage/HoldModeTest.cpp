#include "modules/stage/hold-mode.h"

#include "components/latch.h"
#include "components/phase-timer.h"
#include "fake/FakeControls.h"
#include "modules/stage/event.h"

#include <doctest.h>

namespace test {
namespace stage {
namespace hostage_mode {
using dhe::Latch;
using dhe::PhaseTimer;
using dhe::stage::Event;
using dhe::stage::HoldMode;
static void prepareToExecute(FakeControls &controls) {
  controls.input = []() -> float { return 0.F; };
  controls.duration = []() -> float { return 1.F; };
  controls.output = [](float v) {};
}

TEST_CASE("stage:HoldMode") {
  FakeControls controls{};
  PhaseTimer timer{};
  HoldMode<FakeControls> hold_mode{controls, timer};

  SUBCASE("enter") {
    controls.show_active = [](bool b) {};
    controls.input = []() -> float { return 0.F; };
    controls.output = [](float v) {};

    SUBCASE("activates stage") {
      auto active{false};
      controls.show_active = [&](bool b) { active = b; };
      hold_mode.enter();
      CHECK(active);
    }

    SUBCASE("copies input to output") {
      float input = 7.6344F;
      controls.input = [=]() -> float { return input; };

      float output{-99.F};
      controls.output = [&](float voltage) { output = voltage; };

      hold_mode.enter();

      CHECK_EQ(output, input);
    }

    SUBCASE("resets timer") {
      timer.advance(0.5F);

      hold_mode.enter();

      CHECK_EQ(timer.phase(), 0.F);
    }
  }

  SUBCASE("execute") {
    prepareToExecute(controls);
    SUBCASE("advances phase") {
      auto constexpr duration{3.3498F};
      auto constexpr sampleTime(0.9384F);

      timer.reset();
      controls.duration = []() -> float { return duration; };

      hold_mode.execute(Latch{}, sampleTime);

      CHECK_EQ(timer.phase(), sampleTime / duration);
    }

    SUBCASE("resets timer if retrigger rises") {
      timer.advance(0.5F);

      hold_mode.execute(Latch{true, true}, 0.F);

      CHECK_EQ(timer.phase(), 0.F);
    }

    SUBCASE("returns generated if timer not expired") {
      controls.duration = []() -> float { return 1.F; };
      timer.reset();

      CHECK_EQ(hold_mode.execute(Latch{}, 0.1F), Event::Generated);
    }

    SUBCASE("returns completed if timer expired") {
      controls.duration = []() -> float { return 1.F; };
      timer.reset();
      timer.advance(0.9999F);

      CHECK_EQ(hold_mode.execute(Latch{}, 0.1F), Event::Completed);
    }

    SUBCASE("outputs nothing") {
      hold_mode.execute(Latch{}, 0.F); // Will throw if output called
    }
  }

  SUBCASE("exit deactivates stage") {
    auto active{true};
    controls.show_active = [&](bool b) { active = b; };
    hold_mode.exit();
    CHECK_FALSE(active);
  }
}
} // namespace hostage_mode
} // namespace stage
} // namespace test
