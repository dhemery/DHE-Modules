#include "modules/curve-sequencer-2/StepController.h"

#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "components/Taper.h"
#include "doctest/doctest.h"
#include "fake/Controls.h"
#include "modules/curve-sequencer-2/TriggerMode.h"
#include "modules/curve-sequencer/StepEvent.h"

namespace test {
namespace curve_sequencer_2 {
  namespace step_controller {
    using dhe::Latch;
    using dhe::PhaseTimer;
    using dhe::curve_sequencer_2::StepController;
    using dhe::curve_sequencer::StepEvent;
    using dhe::curve_sequencer_2::TriggerMode;
    using dhe::taper::VariableTaper;

    static auto constexpr risenGate = Latch{true, true};
    static auto constexpr fallenGate = Latch{false, true};
    static auto constexpr highGate = Latch{true, false};
    static auto constexpr lowGate = Latch{false, false};

    static inline void prepareToGenerate(fake::Controls &controls) {
      controls.curvature = [](int s) -> float { return 0.2F; };
      controls.duration = [](int s) -> float { return 0.2F; };
      controls.input = []() -> float { return 0.2F; };
      controls.level = [](int s) -> float { return 0.2F; };
      controls.setOutput = [](float f) {};
      controls.showProgress = [](int step, float progress) {};
      controls.taper = [](int s) -> VariableTaper const * { return dhe::taper::variableTapers[0]; };
    }

    TEST_CASE("curve_sequencer::StepController") {
      fake::Controls controls{};
      PhaseTimer timer{};

      StepController<fake::Controls> stepController{controls, timer};

      SUBCASE("enter activates step at 0 progress") {
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

      SUBCASE("exit deactivates step") {
        controls.getOutput = []() -> float { return 5.5F; };

        auto constexpr step = 1;
        controls.showProgress = [](int step, int progress) {};
        stepController.enter(step);

        auto deactivatedStep = -1;
        controls.showInactive = [&](int step) { deactivatedStep = step; };

        stepController.exit();

        CHECK_EQ(deactivatedStep, step);
      }

      SUBCASE("execute") {
        SUBCASE("when interruptible") {
          SUBCASE("completes without generating if triggered") {}
          SUBCASE("generates if not triggered") {}
        }

        SUBCASE("when uninterruptible") {
          SUBCASE("generates regardless of trigger") {}
        }

        SUBCASE("when advances on EOC") {}
        SUBCASE("when holds on EOC") {}

        SUBCASE("generate") {
          // TODO: start source
          // TODO: track start source
          // TODO: snap start source
          // TODO: track start -> snap start
          // TODO: snap start -> track start

          // TODO: end source
          // TODO: track end source
          // TODO: snap end source
          // TODO: track end -> snap end
          // TODO: snap end -> track end
        }
      }
    }
  } // namespace curve_sequencer_step_controller
} // namespace test
}