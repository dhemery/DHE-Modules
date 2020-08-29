#include "modules/old-curve-sequencer/curve-sequencer.h"

#include "fake/SequenceControls.h"
#include "fake/StepController.h"
#include "fake/StepSelector.h"
#include "modules/old-curve-sequencer/step-event.h"

#include <doctest.h>
#include <functional>

namespace test {
namespace curve_sequencer {
namespace sequencer {
using dhe::Latch;
using dhe::old_curve_sequencer::StepEvent;

template <typename T> inline static auto returns(T t) -> std::function<T()> {
  return [=]() -> T { return t; };
}

using CurveSequencer = dhe::old_curve_sequencer::CurveSequencer<
    fake::SequenceControls, fake::StepSelector, fake::StepController>;

auto constexpr stableLowLatch = Latch{false, false};
auto constexpr fallenLatch = Latch{false, true};
auto constexpr stableHighLatch = Latch{true, false};
auto constexpr risenLatch = Latch{true, true};

TEST_CASE("curve_sequencer::CurveSequencer") {
  fake::SequenceControls controls{};
  fake::StepSelector stepSelector{};
  fake::StepController stepController{};
  CurveSequencer curveSequencer{controls, stepSelector, stepController};

  controls.is_gated = returns(false);
  controls.is_reset = returns(false);

  SUBCASE("when idle") {
    // TODO: Assert that it starts idle.

    SUBCASE("and paused") {
      controls.is_running = returns(false);
      SUBCASE("reset low does nothing") {
        controls.is_reset = returns(false);
        curveSequencer.execute(0.1F); // Will throw if any commands are called
      }

      SUBCASE("reset rise does nothing") {
        controls.is_reset = returns(true);
        curveSequencer.execute(0.1F); // Will throw if any commands are called
      }

      SUBCASE("gate low does nothing") {
        controls.is_gated = returns(false);
        curveSequencer.execute(0.1F); // Will throw if any commands are called
      }

      SUBCASE("gate rise does nothing") {
        controls.is_gated = returns(true);
        curveSequencer.execute(0.1F); // Will throw if any commands are called
      }
    }

    SUBCASE("and running") {
      controls.is_running = returns(true);

      SUBCASE("reset high copies input voltage to output") {
        auto constexpr input{0.12938F};
        controls.is_reset = returns(true);
        controls.input = returns(input);

        controls.output = [=](float output) { CHECK_EQ(output, input); };

        curveSequencer.execute(0.1F);
      }

      SUBCASE("reset low does nothing") {
        controls.is_reset = returns(false);

        curveSequencer.execute(0.1F); // Will throw if any commands are called
      }

      SUBCASE("gate low does nothing") {
        controls.is_gated = returns(false);

        curveSequencer.execute(0.1F); // Will throw if any commands are called
      }

      SUBCASE("gate rise") {
        controls.is_gated = returns(true);

        SUBCASE("executes first enabled step with gate edge cleared") {
          auto constexpr firstEnabledStep{3};
          stepSelector.first = returns(firstEnabledStep);

          auto constexpr sampleTime{0.39947F};

          stepController.enter = [=](float enteredStep) {
            CHECK_EQ(enteredStep, firstEnabledStep);
          };

          stepController.execute = [=](Latch const &executedGateLatch,
                                       float executedSampleTime) -> StepEvent {
            CHECK_EQ(executedGateLatch, stableHighLatch);
            CHECK_EQ(executedSampleTime, sampleTime);
            return StepEvent::Generated;
          };

          curveSequencer.execute(sampleTime);
        }

        SUBCASE("does nothing if no step is enabled") {
          stepSelector.first = returns(-1);
          curveSequencer.execute(0.1F); // Will throw if any commands are called
        }
      }
    }
  }

  SUBCASE("when active") {
    controls.is_running = returns(true);
    int constexpr stepSelectorFirstStep{2};

    stepSelector.first = returns(stepSelectorFirstStep);

    stepController.enter = [=](int enteredStep) {
      REQUIRE_EQ(enteredStep, stepSelectorFirstStep);
    };

    stepController.execute = [&](Latch const &executedGateLatch,
                                 float st) -> StepEvent {
      REQUIRE_EQ(executedGateLatch, stableHighLatch);
      return StepEvent::Generated;
    };
    controls.is_gated = returns(true);

    curveSequencer.execute(0.F);

    // Now that a step is active, pause.
    controls.is_running = returns(false);

    // Forbid further calls
    stepSelector.first = std::function<int()>{};
    stepController.enter = std::function<void(int)>{};

    SUBCASE("and paused") {
      controls.is_running = returns(false);

      SUBCASE("gate low does nothing") {
        controls.is_gated = returns(false);
        curveSequencer.execute(0.F); // Will throw if any commands are called
      }

      SUBCASE("gate rise does nothing") {
        controls.is_gated = returns(true);
        curveSequencer.execute(0.F); // Will throw if any commands are called
      }

      SUBCASE("reset low does nothing") {
        controls.is_reset = returns(false);
        curveSequencer.execute(0.F); // Will throw if any commands are called
      }

      SUBCASE("reset rise exits active step and does nothing else") {
        controls.is_reset = returns(true);
        auto exitCalled = bool{};
        stepController.exit = [&]() { exitCalled = true; };

        curveSequencer.execute(
            0.F); // Will throw if any command except exit is called

        CHECK(exitCalled);
      }
    }

    SUBCASE("and running") {
      controls.is_running = returns(true);

      SUBCASE("executes active step with gate state") {
        auto constexpr sampleTime{0.34901F};

        controls.is_gated = returns(true);
        stepController.execute = [&](Latch const &executedGateLatch,
                                     float st) -> StepEvent {
          REQUIRE_EQ(executedGateLatch, stableHighLatch);
          return StepEvent::Generated;
        };
        curveSequencer.execute(sampleTime);

        controls.is_gated = returns(false);
        stepController.execute = [&](Latch const &executedGateLatch,
                                     float st) -> StepEvent {
          REQUIRE_EQ(executedGateLatch, fallenLatch);
          return StepEvent::Generated;
        };
        curveSequencer.execute(sampleTime);

        controls.is_gated = returns(false);
        stepController.execute = [&](Latch const &executedGateLatch,
                                     float st) -> StepEvent {
          REQUIRE_EQ(executedGateLatch, stableLowLatch);
          return StepEvent::Generated;
        };
        curveSequencer.execute(sampleTime);

        controls.is_gated = returns(true);
        stepController.execute = [&](Latch const &executedGateLatch,
                                     float st) -> StepEvent {
          REQUIRE_EQ(executedGateLatch, risenLatch);
          return StepEvent::Generated;
        };
        curveSequencer.execute(sampleTime);
      }

      SUBCASE("if active step completes") {
        stepController.execute = [](Latch const &l, float f) -> StepEvent {
          return StepEvent::Completed;
        };
        auto controlsIsLooping = bool{};
        controls.is_looping = [&]() -> bool { return controlsIsLooping; };

        SUBCASE("seeks successor with active step and loop state") {
          stepController.enter = [](int s) {};

          auto secondStep{stepSelectorFirstStep + 3};
          stepSelector.successor = [&](int successorStartStep,
                                       bool successorIsLooping) -> int {
            CHECK_EQ(successorStartStep, stepSelectorFirstStep);
            CHECK_EQ(successorIsLooping, controlsIsLooping);
            return secondStep;
          };
          curveSequencer.execute(0.F);

          controlsIsLooping = true;
          auto thirdStep{secondStep + 2};
          stepSelector.successor = [&](int successorStartStep,
                                       bool successorIsLooping) -> int {
            CHECK_EQ(successorStartStep, secondStep);
            CHECK_EQ(successorIsLooping, controlsIsLooping);
            return thirdStep;
          };
          curveSequencer.execute(0.F);
        }

        SUBCASE("enters successor step") {
          auto secondStep{stepSelectorFirstStep + 2};
          stepSelector.successor = [=](int s, bool b) -> int {
            return secondStep;
          };
          stepController.enter = [=](int enteredStep) {
            CHECK_EQ(enteredStep, secondStep);
          };

          curveSequencer.execute(0.F);

          auto thirdStep{secondStep + 3};
          stepSelector.successor = [=](int s, bool b) -> int {
            return thirdStep;
          };
          stepController.enter = [=](int enteredStep) {
            CHECK_EQ(enteredStep, thirdStep);
          };

          curveSequencer.execute(0.F);
        }

        SUBCASE("does nothing if no successor") {
          stepSelector.successor = [](int s, bool l) -> int { return -1; };
          curveSequencer.execute(0.F);
        }
      }

      SUBCASE("if reset rises") {
        controls.is_reset = returns(true);

        SUBCASE("copies input to output") {
          stepController.exit = []() {};
          auto input = 0.6233F;
          auto output = float{};

          controls.input = returns(input);

          controls.output = [&](float voltage) { output = voltage; };

          curveSequencer.execute(0.F);
          CHECK_EQ(output, input);
        }

        SUBCASE("exits active step") {
          controls.input = returns(0.34F);
          controls.output = [](float voltage) {};

          auto calledExit = bool{};
          stepController.exit = [&]() { calledExit = true; };

          curveSequencer.execute(0.F);
          CHECK(calledExit);
        }
      }
    }
  }
}
} // namespace sequencer
} // namespace curve_sequencer
} // namespace test
