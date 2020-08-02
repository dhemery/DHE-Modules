#include "modules/curve-sequencer/CurveSequencer.h"

#include "doctest/doctest.h"
#include "fake/FakeControls.h"
#include "fake/FakeStepController.h"
#include "fake/FakeStepSelector.h"

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

  TEST_CASE("CurveSequencer") {
    FakeControls controls{};
    FakeStepSelector stepSelector{};
    FakeStepController stepController{};
    CurveSequencer<FakeControls, FakeStepSelector, FakeStepController> curveSequencer{controls, stepSelector,
                                                                                      stepController};

    controls.isGated = returns(false);
    controls.isReset = returns(false);

    // TODO: Test that a curve sequencer is initially idle

    SUBCASE("when paused") {
      controls.isRunning = returns(false);
      REQUIRE_FALSE(controls.isRunning());

      SUBCASE("and idle") {
        curveSequencer.execute(0.1F);

        SUBCASE("reset low does nothing") {
          controls.isReset = returns(false);
          curveSequencer.execute(0.1F); // Will throw if any commands are called
        }

        SUBCASE("reset rise does nothing") {
          controls.isReset = returns(true);
          curveSequencer.execute(0.1F); // Will throw if any commands are called
        }

        SUBCASE("gate low does nothing") {
          controls.isGated = returns(false);
          curveSequencer.execute(0.1F); // Will throw if any commands are called
        }

        SUBCASE("gate rise does nothing") {
          controls.isGated = returns(true);
          curveSequencer.execute(0.1F); // Will throw if any commands are called
        }
      }
    }

    SUBCASE("when running") {
      controls.isRunning = returns(true);

      SUBCASE("and idle") {
        curveSequencer.execute(0.1F);

        SUBCASE("reset high copies input voltage to output") {
          auto constexpr input{0.12938F};
          controls.isReset = returns(true);
          controls.input = returns(input);

          controls.output = [=](float output) { CHECK_EQ(output, input); };

          curveSequencer.execute(0.1F);
        }

        SUBCASE("reset low does nothing") {
          controls.isReset = returns(false);

          curveSequencer.execute(0.1F); // Will throw if any commands are called
        }

        SUBCASE("gate low does nothing") {
          controls.isGated = returns(false);

          curveSequencer.execute(0.1F); // Will throw if any commands are called
        }

        SUBCASE("gate rise") {
          controls.isGated = returns(true);

          SUBCASE("executes first enabled step with gate edge cleared") {
            auto constexpr firstEnabledStep{3};
            stepSelector.first = returns(firstEnabledStep);

            auto constexpr sampleTime{0.39947F};

            stepController.enter = [=](float enteredStep) { CHECK_EQ(enteredStep, firstEnabledStep); };

            stepController.execute = [=](Latch const &executedGateLatch, float executedSampleTime) -> StepEvent {
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

      SUBCASE("and active") {
        int constexpr stepSelectorFirstStep{2};

        stepSelector.first = returns(stepSelectorFirstStep);

        stepController.enter = [=](int enteredStep) { REQUIRE_EQ(enteredStep, stepSelectorFirstStep); };

        stepController.execute = [&](Latch const &executedGateLatch, float st) -> StepEvent {
          REQUIRE_EQ(executedGateLatch, stableHighLatch);
          return StepEvent::Generated;
        };
        controls.isGated = returns(true);

        curveSequencer.execute(0.F);

        // Forbid further calls
        stepSelector.first = std::function<int()>{};
        stepController.enter = std::function<void(int)>{};

        SUBCASE("executes active step with gate state") {
          auto constexpr sampleTime{0.34901F};

          controls.isGated = returns(true);
          stepController.execute = [&](Latch const &executedGateLatch, float st) -> StepEvent {
            REQUIRE_EQ(executedGateLatch, stableHighLatch);
            return StepEvent::Generated;
          };
          curveSequencer.execute(sampleTime);

          controls.isGated = returns(false);
          stepController.execute = [&](Latch const &executedGateLatch, float st) -> StepEvent {
            REQUIRE_EQ(executedGateLatch, fallenLatch);
            return StepEvent::Generated;
          };
          curveSequencer.execute(sampleTime);

          controls.isGated = returns(false);
          stepController.execute = [&](Latch const &executedGateLatch, float st) -> StepEvent {
            REQUIRE_EQ(executedGateLatch, stableLowLatch);
            return StepEvent::Generated;
          };
          curveSequencer.execute(sampleTime);

          controls.isGated = returns(true);
          stepController.execute = [&](Latch const &executedGateLatch, float st) -> StepEvent {
            REQUIRE_EQ(executedGateLatch, risenLatch);
            return StepEvent::Generated;
          };
          curveSequencer.execute(sampleTime);
        }

        SUBCASE("if active step completes") {
          stepController.execute = [](Latch const &l, float f) -> StepEvent { return StepEvent::Completed; };
          auto controlsIsLooping = bool{};
          controls.isLooping = [&]() -> bool { return controlsIsLooping; };

          SUBCASE("seeks successor with active step and loop state") {
            stepController.enter = [](int s) {};

            auto secondStep{stepSelectorFirstStep + 3};
            stepSelector.successor = [&](int successorStartStep, bool successorIsLooping) -> int {
              CHECK_EQ(successorStartStep, stepSelectorFirstStep);
              CHECK_EQ(successorIsLooping, controlsIsLooping);
              return secondStep;
            };
            curveSequencer.execute(0.F);

            controlsIsLooping = true;
            auto thirdStep{secondStep + 2};
            stepSelector.successor = [&](int successorStartStep, bool successorIsLooping) -> int {
              CHECK_EQ(successorStartStep, secondStep);
              CHECK_EQ(successorIsLooping, controlsIsLooping);
              return thirdStep;
            };
            curveSequencer.execute(0.F);
          }

          SUBCASE("enters successor step") {
            auto secondStep{stepSelectorFirstStep + 2};
            stepSelector.successor = [=](int s, bool b) -> int { return secondStep; };
            stepController.enter = [=](int enteredStep) { CHECK_EQ(enteredStep, secondStep); };

            curveSequencer.execute(0.F);

            auto thirdStep{secondStep + 3};
            stepSelector.successor = [=](int s, bool b) -> int { return thirdStep; };
            stepController.enter = [=](int enteredStep) { CHECK_EQ(enteredStep, thirdStep); };

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

            controls.output = [&](float voltage) {
              output = voltage;
            };

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

    /*

    class PausedActiveCurveSequencer : public CurveSequencerTest {
    protected:
      int const activeStep{3};

      void SetUp() override {
        CurveSequencerTest::SetUp();
        givenRun(true);

        ON_CALL(stepSelector, first()).WillByDefault(Return(activeStep));
        ON_CALL(stepController, execute(A<Latch const &>(), A<float>())).WillByDefault(Return(StepEvent::Generated));

        givenGate(true);
        curveSequencer.execute(0.F);

        givenRun(false); // PausedActive = not running, but a step in progress
      }
    };

    TEST_F(PausedActiveCurveSequencer, gateLow_doesNothing) {
      givenGate(false);

      expectNoCommands();

      curveSequencer.execute(0.1F);
    }

    TEST_F(PausedActiveCurveSequencer, gateRise_doesNothing) {
      givenGate(true);

      expectNoCommands();

      curveSequencer.execute(0.1F);
    }

    TEST_F(PausedActiveCurveSequencer, resetLow_doesNothing) {
      givenReset(false);

      expectNoCommands();

      curveSequencer.execute(0.F);
    }

    TEST_F(PausedActiveCurveSequencer, resetRise_exitsActiveStep_andDoesNothingElse) {
      givenReset(true);

      expectNoCommands();
      EXPECT_CALL(stepController, exit()).Times(1);

      curveSequencer.execute(0.123F);
    }
    */
  }
} // namespace curve_sequencer
} // namespace test
