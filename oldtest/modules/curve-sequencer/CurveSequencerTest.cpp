namespace test {
namespace curve_sequencer {

template <typename T> inline static auto returns(T t) -> std::function<T()> {
  return [=]() -> T { return t; };
}

using dhe::curve_sequencer::CurveSequencer;

auto constexpr stableLowLatch = Latch{false, false};
auto constexpr fallenLatch = Latch{false, true};
auto constexpr stableHighLatch = Latch{true, false};
auto constexpr risenLatch = Latch{true, true};

TEST_CASE("curve_sequencer::CurveSequencer") {
  FakeControls controls{};
  FakeStepSelector stepSelector{};
  FakeStepController stepController{};
  CurveSequencer<FakeControls, FakeStepSelector, FakeStepController>
      curveSequencer{controls, stepSelector, stepController};

  controls.isGated = returns(false);
  controls.isReset = returns(false);

  SUBCASE("when active") {
    controls.isRunning = returns(true);
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
    controls.isGated = returns(true);

    curveSequencer.execute(0.F);

    // Now that a step is active, pause.
    controls.isRunning = returns(false);

    // Forbid further calls
    stepSelector.first = std::function<int()>{};
    stepController.enter = std::function<void(int)>{};

    SUBCASE("and paused") {
      controls.isRunning = returns(false);

      SUBCASE("gate low does nothing") {
        controls.isGated = returns(false);
        curveSequencer.execute(0.F); // Will throw if any commands are called
      }

      SUBCASE("gate rise does nothing") {
        controls.isGated = returns(true);
        curveSequencer.execute(0.F); // Will throw if any commands are called
      }

      SUBCASE("reset low does nothing") {
        controls.isReset = returns(false);
        curveSequencer.execute(0.F); // Will throw if any commands are called
      }

      SUBCASE("reset rise exits active step and does nothing else") {
        controls.isReset = returns(true);
        auto exitCalled = bool{};
        stepController.exit = [&]() { exitCalled = true; };

        curveSequencer.execute(
            0.F); // Will throw if any command except exit is called

        CHECK(exitCalled);
      }
    }

    SUBCASE("and running") {
      controls.isRunning = returns(true);

      SUBCASE("executes active step with gate state") {
        auto constexpr sampleTime{0.34901F};

        controls.isGated = returns(true);
        stepController.execute = [&](Latch const &executedGateLatch,
                                     float st) -> StepEvent {
          REQUIRE_EQ(executedGateLatch, stableHighLatch);
          return StepEvent::Generated;
        };
        curveSequencer.execute(sampleTime);

        controls.isGated = returns(false);
        stepController.execute = [&](Latch const &executedGateLatch,
                                     float st) -> StepEvent {
          REQUIRE_EQ(executedGateLatch, fallenLatch);
          return StepEvent::Generated;
        };
        curveSequencer.execute(sampleTime);

        controls.isGated = returns(false);
        stepController.execute = [&](Latch const &executedGateLatch,
                                     float st) -> StepEvent {
          REQUIRE_EQ(executedGateLatch, stableLowLatch);
          return StepEvent::Generated;
        };
        curveSequencer.execute(sampleTime);

        controls.isGated = returns(true);
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
        controls.isLooping = [&]() -> bool { return controlsIsLooping; };

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
        controls.isReset = returns(true);

        SUBCASE("copies input to output") {
          stepController.exit = []() {};
          auto input = 0.6233F;
          auto output = float{};

          controls.input = returns(input);

          controls.setOutput = [&](float voltage) { output = voltage; };

          curveSequencer.execute(0.F);
          CHECK_EQ(output, input);
        }

        SUBCASE("exits active step") {
          controls.input = returns(0.34F);
          controls.setOutput = [](float voltage) {};

          auto calledExit = bool{};
          stepController.exit = [&]() { calledExit = true; };

          curveSequencer.execute(0.F);
          CHECK(calledExit);
        }
      }
    }
  }
}
} // namespace curve_sequencer
} // namespace test
