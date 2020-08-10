#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "fake/Controls.h"
#include "fake/Generator.h"
#include "fake/Interrupter.h"
#include "fake/Sustainer.h"
#include "helpers/fake-controls.h"
#include "modules/curve-sequencer-2/StepController.h"
#include "modules/curve-sequencer/StepEvent.h"

#include <doctest.h>

namespace test {
namespace curve_sequencer_2 {
  using test::fake::Generator;
  using test::fake::Interrupter;
  using test::fake::Sustainer;

  namespace step_controller {
    using dhe::Latch;
    using dhe::PhaseTimer;
    using dhe::curve_sequencer::StepEvent;
    using test::fake::forbidden;
    using StepController = dhe::curve_sequencer_2::StepController<fake::Controls, Interrupter, Generator, Sustainer>;

    TEST_CASE("curve_sequencer_2::StepController sustain") {
      fake::Controls controls{};
      Interrupter interrupter{};
      Generator generator{};
      Sustainer sustainer{};
      PhaseTimer timer{};

      StepController stepController{controls, interrupter, generator, sustainer, timer};

      interrupter.isInterrupted = [](int /*s*/, Latch const & /*l*/) -> bool { return false; };

      auto constexpr step{2};
      allowGenerate(controls);
      stepController.enter(step);

      SUBCASE("if generator does not finish") {
        generator.generate = [](int /**/, float /**/) -> bool { return false; };

        SUBCASE("reports generated") {
          auto const result = stepController.execute(Latch{}, 0.F);

          CHECK_EQ(result, StepEvent::Generated);
        }

        SUBCASE("leaves step active") {
          controls.showInactive = [](int s) { throw forbidden("showInactive", s); };
          stepController.execute(Latch{}, 0.F);
        }
      }

      SUBCASE("when generator finishes") {
        generator.generate = [&](int /**/, float /**/) -> bool { return true; };

        SUBCASE("if sustainer is done") {
          sustainer.isDone = [](int /**/, Latch const & /**/) -> bool { return true; };

          SUBCASE("reports completed") {
            controls.showInactive = [](int s) {};

            auto result = stepController.execute(Latch{}, 0.F);

            CHECK_EQ(result, StepEvent::Completed);
          }

          SUBCASE("deactivates step") {
            auto deactivatedStep{-1};
            controls.showInactive = [&](int s) { deactivatedStep = s; };
            stepController.execute(Latch{}, 0.F);
            CHECK_EQ(deactivatedStep, step);
          }
        }

        SUBCASE("if sustainer is not done") {
          sustainer.isDone = [](int /**/, Latch const & /**/) -> bool { return false; };

          SUBCASE("reports generated") {
            auto result = stepController.execute(Latch{}, 0.F);

            CHECK_EQ(result, StepEvent::Generated);
          }

          SUBCASE("leaves step active") {
            controls.showInactive = [](int s) { throw forbidden("showInactive", s); };
            stepController.execute(Latch{}, 0.F);
          }
        }
      }
    }
  } // namespace step_controller
} // namespace curve_sequencer_2
} // namespace test
