#include "modules/stage/generate-mode.h"

#include "components/latch.h"
#include "components/phase-timer.h"
#include "components/sigmoid.h"
#include "fake/FakeControls.h"
#include "modules/stage/event.h"

#include <doctest.h>

namespace test {
namespace stage {
namespace generate_mode {

using dhe::Latch;
using dhe::PhaseTimer;
using dhe::stage::Event;
using dhe::stage::GenerateMode;

static inline void prepare_to_execute(FakeControls &controls) {
  controls.curvature = []() -> float { return 0.F; };
  controls.duration = []() -> float { return 1.F; };
  controls.input = []() -> float { return 0.F; };
  controls.level = []() -> float { return 0.F; };
  controls.output = [](float v) {};
  controls.taper = []() -> dhe::sigmoid::Taper const * {
    return dhe::sigmoid::tapers[0];
  };
}

TEST_CASE("stage::GenerateMode") {
  FakeControls controls{};
  PhaseTimer timer{};
  GenerateMode<FakeControls, PhaseTimer> generate_mode{controls, timer};

  SUBCASE("enter") {
    controls.show_active = [](bool b) {};
    controls.input = []() -> float { return 0.F; };

    SUBCASE("activates stage") {
      auto active{false};
      controls.show_active = [&](bool b) { active = b; };
      generate_mode.enter();
      CHECK(active);
    }

    SUBCASE("resets timer") {
      timer.advance(0.5F);

      generate_mode.enter();

      CHECK_EQ(timer.phase(), 0.F);
    }

    SUBCASE("sets curve starting voltage to input voltage") {
      auto constexpr input{5.3948F};
      controls.input = []() -> float { return input; };

      generate_mode.enter();

      prepare_to_execute(controls);

      auto output{-99.F};
      controls.output = [&](float v) { output = v; };

      // After enter(), phase is 0. Executing with a sample time of 0 will leave
      // the phase at 0. At phase 0, execute outputs the captured starting
      // voltage, which is what we want to verify.
      generate_mode.execute(Latch{}, 0.F);

      CHECK_EQ(output, input);
    }
  }

  SUBCASE("execute") {
    prepare_to_execute(controls);

    SUBCASE("advances phase") {
      auto constexpr duration{3.3498F};
      auto constexpr sample_time(0.9384F);

      timer.reset();
      controls.duration = []() -> float { return duration; };

      generate_mode.execute(Latch{}, sample_time);

      CHECK_EQ(timer.phase(), sample_time / duration);
    }

    SUBCASE("if retrigger rises") {
      SUBCASE("resets timer") {
        timer.advance(0.5F);

        generate_mode.execute(Latch{true, true}, 0.F);

        CHECK_EQ(timer.phase(), 0.F);
      }

      SUBCASE("sets curve starting voltage to input voltage") {
        controls.level = []() -> float { return 9.F; };

        auto constexpr input{5.3948F};
        controls.input = []() -> float { return input; };

        auto output{-99.F};
        controls.output = [&](float v) { output = v; };

        // If retrigger rises, execute resets the timer. Executing with a sample
        // time of 0 will leave the phase at 0. At phase 0, execute outputs the
        // captured starting voltage, which is what we want to verify.
        generate_mode.execute(Latch{true, true}, 0.F);

        CHECK_EQ(output, input);
      }
    }

    SUBCASE("outputs curve voltage") {
      timer.reset();

      // Configure linear curve rising from 4V to 6V over 1s.
      controls.curvature = []() -> float {
        return 0.F;
      }; // 0 curvature -> linear curve
      controls.duration = []() -> float { return 1.F; }; // 1s ramp
      controls.input = []() -> float { return 4.F; };    // Start curve at 4V
      controls.level = []() -> float { return 6.F; };    // End curve at 6V
      // The full range of the curve is 2V = 6V - 4V.

      // Sample time is 1/10 of duration. So execute will raise the output by
      // 0.2V (1/10 of the full curve).
      auto constexpr sample_time = 0.1F;

      controls.show_active = [](bool b) {};
      generate_mode.enter(); // To set starting voltage

      auto output{-99.F};
      controls.output = [&](float v) { output = v; };

      generate_mode.execute(Latch{}, sample_time);

      // Execute must raise output by 0.2V from 4V to 4.2V.
      CHECK_EQ(output, 4.2F);
    }

    SUBCASE("returns generated if timer not expired") {
      controls.duration = []() -> float { return 1.F; };
      timer.reset();

      CHECK_EQ(generate_mode.execute(Latch{}, 0.1F), Event::Generated);
    }

    SUBCASE("returns completed if timer expired") {
      controls.duration = []() -> float { return 1.F; };
      timer.reset();
      timer.advance(0.9999F);

      CHECK_EQ(generate_mode.execute(Latch{}, 0.1F), Event::Completed);
    }
  }

  SUBCASE("exit deactivates stage") {
    auto active{true};
    controls.show_active = [&](bool b) { active = b; };
    generate_mode.exit();
    CHECK_FALSE(active);
  }
}
} // namespace generate_mode
} // namespace stage
} // namespace test
