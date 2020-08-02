#include "modules/stage/HoldMode.h"

#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "doctest/doctest.h"
#include "fake/FakeControls.h"
#include "modules/stage/Event.h"

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
      HoldMode<FakeControls> holdMode{controls, timer};

      SUBCASE("enter") {
        controls.showActive = [](bool b) {};
        controls.input = []() -> float { return 0.F; };
        controls.output = [](float v) {};

        SUBCASE("activates stage") {
          auto active{false};
          controls.showActive = [&](bool b) { active = b; };
          holdMode.enter();
          CHECK(active);
        }

        SUBCASE("copies input to output") {
          float input = 7.6344F;
          controls.input = [=]() -> float { return input; };

          float output{-99.F};
          controls.output = [&](float voltage) { output = voltage; };

          holdMode.enter();

          CHECK_EQ(output, input);
        }

        SUBCASE("resets timer") {
          timer.advance(0.5F);

          holdMode.enter();

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

          holdMode.execute(Latch{}, sampleTime);

          CHECK_EQ(timer.phase(), sampleTime / duration);
        }

        SUBCASE("resets timer if retrigger rises") {
          timer.advance(0.5F);

          holdMode.execute(Latch{true, true}, 0.F);

          CHECK_EQ(timer.phase(), 0.F);
        }

        SUBCASE("returns generated if timer not expired") {
          controls.duration = []() -> float { return 1.F; };
          timer.reset();

          CHECK_EQ(holdMode.execute(Latch{}, 0.1F), Event::Generated);
        }

        SUBCASE("returns completed if timer expired") {
          controls.duration = []() -> float { return 1.F; };
          timer.reset();
          timer.advance(0.9999F);

          CHECK_EQ(holdMode.execute(Latch{}, 0.1F), Event::Completed);
        }

        SUBCASE("outputs nothing") {
          holdMode.execute(Latch{}, 0.F); // Will throw if output called
        }
      }

      SUBCASE("exit deactivates stage") {
        auto active{true};
        controls.showActive = [&](bool b) { active = b; };
        holdMode.exit();
        CHECK_FALSE(active);
      }
    }
  } // namespace hostage_mode
} // namespace stage
} // namespace test
