#include "modules/curve-sequencer-2/StepController.h"

#include "components/Latch.h"
#include "modules/curve-sequencer/StepEvent.h"

#include <doctest.h>

namespace test {
namespace curve_sequencer_2 {
  namespace step_controller {
    using dhe::Latch;
    using dhe::curve_sequencer::StepEvent;

    struct Generator {
      std::function<void(int)> start = [](int s) { throw "start " + std::to_string(s); };
      std::function<bool(float)> generate{[](float t) -> bool { throw "generate " + std::to_string(t); }};
      std::function<void()> stop = []() { throw "stop"; };
    };

    struct Interrupter {
      std::function<bool(int, Latch const &)> isInterrupted{
          [](int s, Latch const &l) -> bool { throw "isInterrupted " + std::to_string(s) + ", " + l.str(); }};
    };

    struct Sustainer {
      std::function<bool(int, Latch const &)> isDone{
          [](int s, Latch const &l) -> bool { throw "isDone" + std::to_string(s) + ", " + l.str(); }};
    };

    using StepController = dhe::curve_sequencer_2::StepController<Interrupter, Generator, Sustainer>;

    static inline void enter(StepController &stepController, Generator &generator, int step) {
      generator.start = [](int /**/) {};
      stepController.enter(step);
      generator.start = [](int s) { throw "start " + std::to_string(s); };
    }

    TEST_CASE("curve_sequencer_2::StepController") {
      Interrupter interrupter{};
      Generator generator{};
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

      SUBCASE("execute") {
        SUBCASE("if interrupted") {
          SUBCASE("completes without generating") {
            interrupter.isInterrupted = [](int /**/, Latch const & /**/) -> bool { return true; };
            generator.generate = [](float t) -> bool { throw std::string{"generate "} + std::to_string(t); };

            auto constexpr step{7};
            enter(stepController, generator, step);

            auto stopped{false};
            generator.stop = [&]() { stopped = true; };

            auto result = stepController.execute(Latch{}, 0.F);
            CHECK_EQ(result, StepEvent::Completed);
            CHECK(stopped);
          }
        }

        SUBCASE("if not interrupted") {
          interrupter.isInterrupted = [](int /**/, Latch const & /**/) -> bool { return false; };

          SUBCASE("generates") {
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

        SUBCASE("when generator finishes") {
          interrupter.isInterrupted = [](int /**/, Latch const & /**/) -> bool { return false; };
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

        SUBCASE("if generator does not finish") {
          interrupter.isInterrupted = [](int /*s*/, Latch const & /*l*/) -> bool { return false; };
          generator.generate = [](float /**/) -> bool { return false; };

          SUBCASE("leaves generator running and reports generated") {
            generator.stop = []() { throw "stop"; };

            auto const result = stepController.execute(Latch{}, 0.F);

            CHECK_EQ(result, StepEvent::Generated);
          }
        }
      }
    }
  } // namespace step_controller
} // namespace curve_sequencer_2
} // namespace test
