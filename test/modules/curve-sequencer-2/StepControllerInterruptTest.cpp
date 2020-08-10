#include "components/Latch.h"
#include "fake/Generator.h"
#include "fake/Interrupter.h"
#include "fake/Sustainer.h"
#include "modules/curve-sequencer-2/StepController.h"
#include "modules/curve-sequencer/StepEvent.h"

#include <doctest.h>

namespace test {
namespace curve_sequencer_2 {
  namespace step_controller {
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

    TEST_CASE("curve_sequencer_2::StepController interrupt") {
      Interrupter interrupter{};
      Generator generator{};
      Sustainer sustainer{};

      StepController stepController{interrupter, generator, sustainer};

      SUBCASE("completes without generating if interrupted") {
        interrupter.isInterrupted = [](int /**/, Latch const & /**/) -> bool { return true; };
        generator.generate = [](float t) -> bool { throw forbidden("generate", t); };
        auto constexpr step{7};

        enter(stepController, generator, step);

        auto stopped{false};
        generator.stop = [&]() { stopped = true; };

        auto result = stepController.execute(Latch{}, 0.F);
        CHECK_EQ(result, StepEvent::Completed);
        CHECK(stopped);
      }

      SUBCASE("generates if not interrupted") {
        interrupter.isInterrupted = [](int /**/, Latch const & /**/) -> bool { return false; };
        sustainer.isDone = [](int /**/, Latch const & /**/) -> bool { return false; };

        auto constexpr step{3};
        enter(stepController, generator, step);

        float generatedSampleTime(-1.F);
        generator.generate = [&](float t) -> bool {
          generatedSampleTime = t;
          return false;
        };

        auto constexpr sampleTime{0.117F};
        auto result = stepController.execute(Latch{}, sampleTime);

        CHECK_EQ(result, StepEvent::Generated);
        CHECK_EQ(generatedSampleTime, sampleTime);
      }
    }
  } // namespace step_controller
} // namespace curve_sequencer_2
} // namespace test
