#include "fake/Generator.h"
#include "fake/Interrupter.h"
#include "fake/Sustainer.h"
#include "modules/curve-sequencer-2/StepController.h"

#include <doctest.h>

namespace test {
namespace curve_sequencer_2 {

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

    TEST_CASE("curve_sequencer_2::StepController progress") {
      Generator generator{};
      Interrupter interrupter{};
      Sustainer sustainer{};

      StepController stepController{interrupter, generator, sustainer};

      SUBCASE("enter starts generator") {
        auto constexpr step = 7;
        auto startedStep = int{-1};
        generator.start = [&](int s) { startedStep = s; };

        stepController.enter(step);

        CHECK_EQ(startedStep, step);
      }

      SUBCASE("exit stops generator") {
        auto constexpr step = 1;
        enter(stepController, generator, step);

        auto stopped{false};
        generator.stop = [&]() { stopped = true; };

        stepController.exit();

        CHECK(stopped);
      }
    }
  } // namespace step_controller
} // namespace curve_sequencer_2
} // namespace test
