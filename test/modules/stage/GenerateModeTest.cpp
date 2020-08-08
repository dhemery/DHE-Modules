#include "modules/stage/GenerateMode.h"

#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "components/Taper.h"
#include "fake/FakeControls.h"
#include "modules/stage/Event.h"

#include <doctest.h>

namespace test {
namespace stage {
  namespace generate_mode {

    using dhe::Latch;
    using dhe::PhaseTimer;
    using dhe::stage::Event;
    using dhe::stage::GenerateMode;

    static inline void prepareToExecute(FakeControls &controls) {
      controls.curvature = []() -> float { return 0.F; };
      controls.duration = []() -> float { return 1.F; };
      controls.input = []() -> float { return 0.F; };
      controls.level = []() -> float { return 0.F; };
      controls.output = [](float v) {};
      controls.taper = []() -> dhe::taper::VariableTaper const * { return dhe::taper::variableTapers[0]; };
    }

    TEST_CASE("stage::GenerateMode") {
      FakeControls controls{};
      PhaseTimer timer{};
      GenerateMode<FakeControls, PhaseTimer> generateMode{controls, timer};

      SUBCASE("enter") {
        controls.showActive = [](bool b) {};
        controls.input = []() -> float { return 0.F; };

        SUBCASE("activates stage") {
          auto active{false};
          controls.showActive = [&](bool b) { active = b; };
          generateMode.enter();
          CHECK(active);
        }

        SUBCASE("resets timer") {
          timer.advance(0.5F);

          generateMode.enter();

          CHECK_EQ(timer.phase(), 0.F);
        }

        SUBCASE("sets curve starting voltage to input voltage") {
          auto constexpr input{5.3948F};
          controls.input = []() -> float { return input; };

          generateMode.enter();

          prepareToExecute(controls);

          auto output{-99.F};
          controls.output = [&](float v) { output = v; };

          // After enter(), phase is 0. Executing with a sample time of 0 will leave the phase at 0. At phase 0, execute
          // outputs the captured starting voltage, which is what we want to verify.
          generateMode.execute(Latch{}, 0.F);

          CHECK_EQ(output, input);
        }
      }

      SUBCASE("execute") {
        prepareToExecute(controls);

        SUBCASE("advances phase") {
          auto constexpr duration{3.3498F};
          auto constexpr sampleTime(0.9384F);

          timer.reset();
          controls.duration = []() -> float { return duration; };

          generateMode.execute(Latch{}, sampleTime);

          CHECK_EQ(timer.phase(), sampleTime / duration);
        }

        SUBCASE("if retrigger rises") {
          SUBCASE("resets timer") {
            timer.advance(0.5F);

            generateMode.execute(Latch{true, true}, 0.F);

            CHECK_EQ(timer.phase(), 0.F);
          }

          SUBCASE("sets curve starting voltage to input voltage") {
            controls.level = []() -> float { return 9.F; };

            auto constexpr input{5.3948F};
            controls.input = []() -> float { return input; };

            auto output{-99.F};
            controls.output = [&](float v) { output = v; };

            // If retrigger rises, execute resets the timer. Executing with a sample time of 0 will leave the phase at
            // 0. At phase 0, execute outputs the captured starting voltage, which is what we want to verify.
            generateMode.execute(Latch{true, true}, 0.F);

            CHECK_EQ(output, input);
          }
        }

        SUBCASE("outputs curve voltage") {
          timer.reset();

          // Configure linear curve rising from 4V to 6V over 1s.
          controls.curvature = []() -> float { return 0.F; }; // 0 curvature -> linear curve
          controls.duration = []() -> float { return 1.F; };  // 1s ramp
          controls.input = []() -> float { return 4.F; };     // Start curve at 4V
          controls.level = []() -> float { return 6.F; };     // End curve at 6V
          // The full range of the curve is 2V = 6V - 4V.

          // Sample time is 1/10 of duration. So execute will raise the output by 0.2V (1/10 of the full curve).
          auto constexpr sampleTime = 0.1F;

          controls.showActive = [](bool b) {};
          generateMode.enter(); // To set starting voltage

          auto output{-99.F};
          controls.output = [&](float v) { output = v; };

          generateMode.execute(Latch{}, sampleTime);

          // Execute must raise output by 0.2V from 4V to 4.2V.
          CHECK_EQ(output, 4.2F);
        }

        SUBCASE("returns generated if timer not expired") {
          controls.duration = []() -> float { return 1.F; };
          timer.reset();

          CHECK_EQ(generateMode.execute(Latch{}, 0.1F), Event::Generated);
        }

        SUBCASE("returns completed if timer expired") {
          controls.duration = []() -> float { return 1.F; };
          timer.reset();
          timer.advance(0.9999F);

          CHECK_EQ(generateMode.execute(Latch{}, 0.1F), Event::Completed);
        }
      }

      SUBCASE("exit deactivates stage") {
        auto active{true};
        controls.showActive = [&](bool b) { active = b; };
        generateMode.exit();
        CHECK_FALSE(active);
      }
    }
  } // namespace generate_mode
} // namespace stage
} // namespace test
