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

          auto output = 0.F;
          controls.output = [&](float voltage) { output = voltage; };

          curveSequencer.execute(0.1F);

          CHECK_EQ(output, input);
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

            auto enteredStep = float{};
            auto executedGateLatch = Latch{};
            auto executedSampleTime = float{};
            stepController.enter = [&](float s) { enteredStep = s; };
            stepController.execute = [&](Latch const &l, float st) -> StepEvent {
              executedGateLatch = l;
              executedSampleTime = st;
              return StepEvent::Generated;
            };

            auto constexpr sampleTime{0.39947F};

            curveSequencer.execute(sampleTime);

            CHECK_EQ(enteredStep, firstEnabledStep);
            CHECK_EQ(executedGateLatch, stableHighLatch);
            CHECK_EQ(executedSampleTime, sampleTime);
          }

          SUBCASE("does nothing if no step is enabled") {
            stepSelector.first = returns(-1);
            curveSequencer.execute(0.1F); // Will throw if any commands are called
          }
        }
      }

      SUBCASE("and active") {
        int constexpr selectedStep{2};

        stepSelector.first = returns(selectedStep);

        auto activeStep = int{};
        stepController.enter = [&](int s) { activeStep = s; };

        auto executedGateLatch = Latch{};
        auto executedSampleTime = float{};
        stepController.execute = [&](Latch const &l, float st) -> StepEvent {
          executedGateLatch = l;
          executedSampleTime = st;
          return StepEvent::Generated;
        };
        controls.isGated = returns(true);

        curveSequencer.execute(0.F);

        // Forbid further calls
        stepSelector.first = std::function<int()>{};
        stepController.enter = std::function<void(int)>{};

        REQUIRE_EQ(executedGateLatch, stableHighLatch);
        REQUIRE_EQ(activeStep, selectedStep);

        SUBCASE("executes active step with gate state") {
          auto constexpr sampleTime{0.34901F};

          curveSequencer.execute(sampleTime); // Gate stays high
          CHECK_EQ(executedGateLatch, stableHighLatch);

          controls.isGated = returns(false); // Gate falls
          curveSequencer.execute(sampleTime);
          CHECK_EQ(executedGateLatch, fallenLatch);

          controls.isGated = returns(false); // Gate stays low, clears edge
          curveSequencer.execute(sampleTime);
          CHECK_EQ(executedGateLatch, stableLowLatch);

          controls.isGated = returns(true); // Gate rises
          curveSequencer.execute(sampleTime);
          CHECK_EQ(executedGateLatch, risenLatch);
        }

        SUBCASE("if active step completes") {
          stepController.execute = [&](Latch const &l, float f) -> StepEvent { return StepEvent::Completed; };

          SUBCASE("seeks successor with active step and loop state") {
            stepController.enter = [](int s) {};

            controls.isLooping = returns(true);
            auto secondStep{activeStep + 3};
            stepSelector.successor = [&](int startStep, bool isLooping) -> int {
              CHECK_EQ(startStep, activeStep);
              CHECK(isLooping);
              return secondStep;
            };

            curveSequencer.execute(0.F);

            controls.isLooping = returns(false);
            stepSelector.successor = [&](int startStep, bool isLooping) -> int {
              CHECK_EQ(startStep, secondStep);
              CHECK_FALSE(isLooping);
              return startStep + 3;
            };

            curveSequencer.execute(0.F);
          }

          SUBCASE("enters successor step") {
            controls.isLooping = returns(false);

            auto secondStep{activeStep + 2};
            stepSelector.successor = [&](int s, bool b) -> int { return secondStep; };

            auto enteredStep = int{};
            stepController.enter = [&](int s) { enteredStep = s; };

            curveSequencer.execute(0.F);
            CHECK_EQ(enteredStep, secondStep);

            auto thirdStep{secondStep + 3};
            stepSelector.successor = [&](int s, bool b) -> int { return thirdStep; };

            curveSequencer.execute(0.F);
            CHECK_EQ(enteredStep, thirdStep);
          }
        }
      }
    }

    /*

    TEST_F(RunningActiveCurveSequencer, doesNothing_ifActiveStepCompletes_andNoSuccessor) {
      expectNoCommands();

      EXPECT_CALL(stepController, execute(A<Latch const &>(), A<float>())).WillOnce(Return(StepEvent::Completed));
      ON_CALL(stepSelector, successor(activeStep, false)).WillByDefault(Return(-1));

      curveSequencer.execute(0.F);
    }

    TEST_F(RunningActiveCurveSequencer, resetRise_exitsActiveStep) {
      expectNoStepCommands();
      EXPECT_CALL(stepController, exit()).Times(1);

      givenReset(true);
      curveSequencer.execute(0.F);
    }

    TEST_F(RunningActiveCurveSequencer, resetRise_copiesInputVoltageToOutput) {
      givenReset(true);

      auto constexpr input{0.89347F};
      givenInput(input);

      EXPECT_CALL(controls, output(input));

      curveSequencer.execute(0.F);
    }

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
