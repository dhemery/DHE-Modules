#include "modules/old-curve-sequencer/curve-sequencer.h"

#include "fake/SequenceControls.h"
#include "fake/StepController.h"
#include "fake/StepSelector.h"
#include "modules/old-curve-sequencer/step-event.h"

#include <doctest.h>
#include <functional>

namespace test {
namespace old_curve_sequencer {
namespace sequencer {
using dhe::Latch;
using dhe::old_curve_sequencer::StepEvent;

template <typename T> inline static auto returns(T t) -> std::function<T()> {
  return [=]() -> T { return t; };
}

using CurveSequencer = dhe::old_curve_sequencer::CurveSequencer<
    fake::SequenceControls, fake::StepSelector, fake::StepController>;

auto constexpr stableHighLatch = Latch{true, false};

TEST_CASE("old_curve_sequencer::CurveSequencer") {
  fake::SequenceControls controls{};
  fake::StepSelector stepSelector{};
  fake::StepController stepController{};
  CurveSequencer curveSequencer{controls, stepSelector, stepController};

  controls.is_gated = returns(false);
  controls.is_reset = returns(false);

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
