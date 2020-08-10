#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "fake/Controls.h"
#include "fake/Interrupter.h"
#include "fake/Sustainer.h"
#include "modules/curve-sequencer-2/StepController.h"
#include "modules/curve-sequencer/StepEvent.h"

#include <doctest.h>

namespace test {
namespace curve_sequencer_2 {
  namespace step_controller {
    using dhe::Latch;
    using dhe::PhaseTimer;
    using dhe::curve_sequencer::StepEvent;
    using test::fake::Controls;
    using test::fake::forbidden;
    using test::fake::Interrupter;
    using test::fake::Sustainer;
    using StepController = dhe::curve_sequencer_2::StepController<Controls, Interrupter, Sustainer>;

    TEST_CASE("curve_sequencer_2::StepController interrupt") {
      Controls controls{};
      Interrupter interrupter{};
      Sustainer sustainer{};
      PhaseTimer timer{};

      StepController stepController{controls, interrupter, sustainer, timer};

      SUBCASE("completes without generating if interrupted") {
        interrupter.isInterrupted = [](int /*unused*/, Latch const & /*unused*/) -> bool { return true; };
        auto constexpr step{7};

        controls.showProgress = [](int s, float p) {};
        stepController.enter(step);
        controls.showProgress = [](int s, float f) { throw forbidden("showProgress", s, f); };

        auto deactivatedStep{99};
        controls.showInactive = [&](int s) { deactivatedStep = s; };

        auto result = stepController.execute(Latch{}, 0.F);
        CHECK_EQ(result, StepEvent::Completed);
        CHECK_EQ(deactivatedStep, step);
      }

      SUBCASE("generates if not interrupted") {
        interrupter.isInterrupted = [](int /**/, Latch const & /**/) -> bool { return false; };
        sustainer.isDone = [](int /**/, Latch const & /**/) -> bool { return false; };
        allowGenerate(controls);

        auto result = stepController.execute(Latch{}, 0.F);
        CHECK_EQ(result, StepEvent::Generated);
      }
    }
  } // namespace step_controller
} // namespace curve_sequencer_2
} // namespace test
