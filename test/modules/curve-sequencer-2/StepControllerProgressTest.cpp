#include "components/PhaseTimer.h"
#include "fake/Controls.h"
#include "modules/curve-sequencer-2/StepController.h"

#include <doctest.h>
#include <modules/curve-sequencer-2/fake/Interrupter.h>
#include <modules/curve-sequencer-2/fake/Sustainer.h>

namespace test {
namespace curve_sequencer_2 {
  namespace step_controller {
    using dhe::PhaseTimer;
    using test::fake::Interrupter;
    using test::fake::Sustainer;
    using StepController = dhe::curve_sequencer_2::StepController<fake::Controls, Interrupter, Sustainer>;

    TEST_CASE("curve_sequencer_2::StepController progress") {
      fake::Controls controls{};
      Interrupter interrupter{};
      Sustainer sustainer{};
      PhaseTimer timer{};

      StepController stepController{controls, interrupter, sustainer, timer};

      SUBCASE("enter shows 0 progress") {
        controls.getOutput = []() -> float { return 5.5F; };

        auto constexpr step = 7;
        auto shownStep = int{};
        auto shownProgress = -1.F;
        controls.showProgress = [&](int step, float progress) {
          shownStep = step;
          shownProgress = progress;
        };

        stepController.enter(step);

        CHECK_EQ(shownStep, step);
        CHECK_EQ(shownProgress, 0.F);
      }

      SUBCASE("exit dims progress light") {
        controls.getOutput = []() -> float { return 5.5F; };

        auto constexpr step = 1;
        controls.showProgress = [](int s, int p) {};
        stepController.enter(step);

        auto deactivatedStep = -1;
        controls.showInactive = [&](int s) { deactivatedStep = s; };

        stepController.exit();

        CHECK_EQ(deactivatedStep, step);
      }
    }
  } // namespace step_controller
} // namespace curve_sequencer_2
} // namespace test
