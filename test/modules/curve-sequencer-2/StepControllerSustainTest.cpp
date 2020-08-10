#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "fake/Controls.h"
#include "fake/Interrupter.h"
#include "fake/Sustainer.h"
#include "helpers/fake-controls.h"
#include "modules/curve-sequencer-2/StepController.h"
#include "modules/curve-sequencer/StepEvent.h"

#include <doctest.h>

namespace test {
namespace curve_sequencer_2 {
  using test::fake::Interrupter;
  using test::fake::Sustainer;

  namespace step_controller {
    using dhe::Latch;
    using dhe::PhaseTimer;
    using dhe::curve_sequencer::StepEvent;
    using test::fake::forbidden;
    using StepController = dhe::curve_sequencer_2::StepController<fake::Controls, Interrupter, Sustainer>;

    static auto constexpr lowGate = Latch{false, false};

    TEST_CASE("curve_sequencer_2::StepController sustain") {
      fake::Controls controls{};
      Sustainer sustainer{};
      Interrupter interrupter{};
      PhaseTimer timer{};

      StepController stepController{controls, interrupter, sustainer, timer};

      interrupter.isInterrupted = [](int, Latch const &) -> bool { return false; };

      auto constexpr step{2};
      allowGenerate(controls);
      stepController.enter(step);

      SUBCASE("if curve does not complete") {
        timer.reset();
        controls.duration = [](int) -> float { return 1.F; };
        auto constexpr sampleTime{0.1F}; // Not enough to complete the step
        sustainer.isDone = [](int s, Latch const &l) -> bool { throw forbidden("Sustainer.isDone", s, l.isHigh()); };

        SUBCASE("generates") {
          auto output{-441.3F};
          controls.setOutput = [&](float v) { output = v; };
          stepController.execute(lowGate, sampleTime);
          CHECK_EQ(output, controls.endLevel(step));
        }

        SUBCASE("reports generated") {
          auto const result = stepController.execute(lowGate, sampleTime);
          CHECK_EQ(result, StepEvent::Generated);
        }

        SUBCASE("leaves step active") {
          controls.showInactive = [](int s) { throw forbidden("showInactive", s); };
          stepController.execute(lowGate, sampleTime);
        }
      }

      SUBCASE("when curve completes") {
        timer.reset();
        timer.advance(0.99F);
        controls.duration = [](int) -> float { return 1.F; };
        auto constexpr sampleTime{0.1F}; // Enough to complete the step

        SUBCASE("if sustainer is done") {
          sustainer.isDone = [](int, Latch const &) -> bool { return true; };
          controls.showInactive = [](int s) {};

          SUBCASE("generates") {
            auto output{-992.3F};
            controls.setOutput = [&](float v) { output = v; };
            stepController.execute(lowGate, sampleTime);
            CHECK_EQ(output, controls.endLevel(step));
          }

          SUBCASE("reports complete") {
            auto result = stepController.execute(lowGate, sampleTime);
            CHECK_EQ(result, StepEvent::Completed);
          }

          SUBCASE("deactivates step") {
            auto deactivatedStep = -1;
            controls.showInactive = [&](int step) { deactivatedStep = step; };
            stepController.execute(lowGate, sampleTime);
            CHECK_EQ(deactivatedStep, step);
          }
        }

        SUBCASE("if sustainer is not done") {
          sustainer.isDone = [](int, Latch const &) -> bool { return false; };
          SUBCASE("generates") {
            auto output{-441.3F};
            controls.setOutput = [&](float v) { output = v; };
            stepController.execute(lowGate, sampleTime);
            CHECK_EQ(output, controls.endLevel(step));
          }

          SUBCASE("reports generated") {
            auto const result = stepController.execute(lowGate, sampleTime);
            CHECK_EQ(result, StepEvent::Generated);
          }

          SUBCASE("leaves step active") {
            controls.showInactive = [](int s) { throw forbidden("showInactive", s); };
            stepController.execute(lowGate, sampleTime);
          }
        }
      }
    }
  } // namespace step_controller
} // namespace curve_sequencer_2
} // namespace test
