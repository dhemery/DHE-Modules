#include "components/Latch.h"
#include "fake/Generator.h"
#include "fake/Interrupter.h"
#include "fake/Sustainer.h"
#include "helpers/fake-controls.h"
#include "modules/curve-sequencer-2/StepController.h"
#include "modules/curve-sequencer/StepEvent.h"

#include <doctest.h>

namespace test {
namespace curve_sequencer_2 {
  using dhe::Latch;
  using dhe::curve_sequencer::StepEvent;
  using test::fake::forbidden;
  using test::fake::Generator;
  using test::fake::Interrupter;
  using test::fake::Sustainer;
  using StepController = dhe::curve_sequencer_2::StepController<Interrupter, Generator, Sustainer>;

  static inline void enter(StepController &stepController, Generator &generator, int step) {
    generator.start = [](int /**/) {};
    stepController.enter(step);
    generator.start = [](int s) { throw forbidden("start", s); };
  }

  namespace step_controller {
    TEST_CASE("curve_sequencer_2::StepController sustain") {
      Interrupter interrupter{};
      Generator generator{};
      Sustainer sustainer{};

      StepController stepController{interrupter, generator, sustainer};

      interrupter.isInterrupted = [](int /*s*/, Latch const & /*l*/) -> bool { return false; };

      auto constexpr step{2};
      enter(stepController, generator, step);

      SUBCASE("if generator does not finish") {
        generator.generate = [](float /**/) -> bool { return false; };

        SUBCASE("leaves generator running and reports generated") {
          generator.stop = []() { throw "stop"; };

          auto const result = stepController.execute(Latch{}, 0.F);

          CHECK_EQ(result, StepEvent::Generated);
        }

        SUBCASE("leaves generator running") { stepController.execute(Latch{}, 0.F); }
      }

      SUBCASE("when generator finishes") {
        generator.generate = [&](float /**/) -> bool { return true; };

        SUBCASE("if sustainer is done") {
          sustainer.isDone = [](int /**/, Latch const & /**/) -> bool { return true; };

          SUBCASE("stops generator and reports completed") {
            auto stopped{false};
            generator.stop = [&]() { stopped = true; };

            auto result = stepController.execute(Latch{}, 0.F);

            CHECK_EQ(result, StepEvent::Completed);
            CHECK(stopped);
          }
        }

        SUBCASE("if sustainer is not done") {
          sustainer.isDone = [](int /**/, Latch const & /**/) -> bool { return false; };

          SUBCASE("leaves generator running reports generated") {
            generator.stop = []() { throw "stop"; };

            auto result = stepController.execute(Latch{}, 0.F);

            CHECK_EQ(result, StepEvent::Generated);
          }
        }
      }
    }
  } // namespace step_controller
} // namespace curve_sequencer_2
} // namespace test
