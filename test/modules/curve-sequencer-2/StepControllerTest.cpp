#include "modules/curve-sequencer-2/StepController.h"

#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "doctest/doctest.h"
#include "fake/Controls.h"
#include "modules/curve-sequencer-2/TriggerMode.h"
#include "modules/curve-sequencer/StepEvent.h"

#include <vector>

namespace test {
namespace curve_sequencer_2 {
  namespace step_controller {
    using dhe::Latch;
    using dhe::PhaseTimer;
    using dhe::curve_sequencer::StepEvent;
    using dhe::curve_sequencer_2::StepController;
    using dhe::curve_sequencer_2::TriggerMode;

    static auto constexpr risingGate = Latch{true, true};
    static auto constexpr fallingGate = Latch{false, true};
    static auto constexpr highGate = Latch{true, false};
    static auto constexpr lowGate = Latch{false, false};

    auto const gateStates = std::vector<Latch>{risingGate, fallingGate, lowGate, highGate};

    auto const triggerModes = std::vector<TriggerMode>{
        TriggerMode::GateRises,  TriggerMode::GateFalls, TriggerMode::GateChanges,
        TriggerMode::GateIsHigh, TriggerMode::GateIsLow,
    };

    TEST_CASE("curve_sequencer_2::StepController") {
      fake::Controls controls{};
      PhaseTimer timer{};

      StepController<fake::Controls> stepController{controls, timer};

      SUBCASE("enter activates step at 0 progress") {
        controls.getOutput = []() -> float { return 5.5F; };

        auto constexpr step = 7;
        auto shownStep = int{};
        auto shownProgress = -1.F;
        controls.showProgress = [&](int step, float progress) {
          shownStep = step;
          shownProgress = progress;
        };

        stepController.enter(step);

        CHECK_EQ(shownStep, step);
        CHECK_EQ(shownProgress, 0.F);
      }

      SUBCASE("exit deactivates step") {
        controls.getOutput = []() -> float { return 5.5F; };

        auto constexpr step = 1;
        controls.showProgress = [](int s, int p) {};
        stepController.enter(step);

        auto deactivatedStep = -1;
        controls.showInactive = [&](int s) { deactivatedStep = s; };

        stepController.exit();

        CHECK_EQ(deactivatedStep, step);
      }

      SUBCASE("execute") {
        SUBCASE("when interruptible") {
          controls.interruptOnTrigger = stepControlReturning(true);
          controls.advanceOnEndOfCurve = stepControlReturning(false);
          auto constexpr step{7};
          controls.showProgress = [](int s, float p) {};
          stepController.enter(step);
          controls.showProgress = [](int s, float f) { throw forbidden("showProgress", s, f); };

          SUBCASE("completes without generating if triggered") {
            controls.showInactive = [](int s) {};

            SUBCASE("by rising gate") {
              controls.triggerMode = stepControlReturning(TriggerMode::GateRises);
              auto result = stepController.execute(risingGate, 0.F);
              CHECK_EQ(result, StepEvent::Completed);
            }

            SUBCASE("by falling gate") {
              controls.triggerMode = stepControlReturning(TriggerMode::GateFalls);
              auto result = stepController.execute(fallingGate, 0.F);
              CHECK_EQ(result, StepEvent::Completed);
            }

            SUBCASE("by changing gate") {
              controls.triggerMode = stepControlReturning(TriggerMode::GateChanges);
              auto result = stepController.execute(fallingGate, 0.F);
              CHECK_EQ(result, StepEvent::Completed);
              result = stepController.execute(risingGate, 0.F);
              CHECK_EQ(result, StepEvent::Completed);
            }

            SUBCASE("by high gate") {
              controls.triggerMode = stepControlReturning(TriggerMode::GateIsHigh);
              auto result = stepController.execute(highGate, 0.F);
              CHECK_EQ(result, StepEvent::Completed);
              result = stepController.execute(risingGate, 0.F);
              CHECK_EQ(result, StepEvent::Completed);
            }

            SUBCASE("by low gate") {
              controls.triggerMode = stepControlReturning(TriggerMode::GateIsLow);
              auto result = stepController.execute(lowGate, 0.F);
              CHECK_EQ(result, StepEvent::Completed);
              result = stepController.execute(fallingGate, 0.F);
              CHECK_EQ(result, StepEvent::Completed);
            }
          }

          SUBCASE("generates if not triggered") {
            allowGenerate(controls);

            SUBCASE("waiting for rising gate") {
              controls.triggerMode = stepControlReturning(TriggerMode::GateRises);
              auto result = stepController.execute(highGate, 0.F);
              CHECK_EQ(result, StepEvent::Generated);
              result = stepController.execute(fallingGate, 0.F);
              CHECK_EQ(result, StepEvent::Generated);
              result = stepController.execute(lowGate, 0.F);
              CHECK_EQ(result, StepEvent::Generated);
            }

            SUBCASE("waiting for falling gate") {
              controls.triggerMode = stepControlReturning(TriggerMode::GateFalls);
              auto result = stepController.execute(lowGate, 0.F);
              CHECK_EQ(result, StepEvent::Generated);
              result = stepController.execute(highGate, 0.F);
              CHECK_EQ(result, StepEvent::Generated);
              result = stepController.execute(risingGate, 0.F);
              CHECK_EQ(result, StepEvent::Generated);
            }

            SUBCASE("waiting for changing gate") {
              controls.triggerMode = stepControlReturning(TriggerMode::GateChanges);
              auto result = stepController.execute(lowGate, 0.F);
              CHECK_EQ(result, StepEvent::Generated);
              result = stepController.execute(highGate, 0.F);
              CHECK_EQ(result, StepEvent::Generated);
            }

            SUBCASE("waiting for high gate") {
              controls.triggerMode = stepControlReturning(TriggerMode::GateIsHigh);
              auto result = stepController.execute(lowGate, 0.F);
              CHECK_EQ(result, StepEvent::Generated);
              result = stepController.execute(fallingGate, 0.F);
              CHECK_EQ(result, StepEvent::Generated);
            }

            SUBCASE("waiting for low gate") {
              controls.triggerMode = stepControlReturning(TriggerMode::GateIsLow);
              auto result = stepController.execute(highGate, 0.F);
              CHECK_EQ(result, StepEvent::Generated);
              result = stepController.execute(risingGate, 0.F);
              CHECK_EQ(result, StepEvent::Generated);
            }
          }
        }

        SUBCASE("when uninterruptible") {
          allowGenerate(controls);
          controls.interruptOnTrigger = stepControlReturning(false);

          SUBCASE("generates regardless of trigger") {
            std::for_each(triggerModes.cbegin(), triggerModes.cend(),
                          [&controls, &stepController](TriggerMode const triggerMode) {
                            std::for_each(gateStates.cbegin(), gateStates.cend(),
                                          [&controls, &stepController, triggerMode](Latch const &gateState) {
                                            controls.triggerMode = stepControlReturning(triggerMode);
                                            auto result = stepController.execute(gateState, 0.F);
                                            CHECK_EQ(result, StepEvent::Generated);
                                          });
                          });
          }
        }

        SUBCASE("when advances on EOC") {
          auto constexpr step{2};
          allowGenerate(controls);
          stepController.enter(step);
          controls.advanceOnEndOfCurve = stepControlReturning(true);

          SUBCASE("if curve completes") {
            timer.reset();
            timer.advance(0.99F);
            controls.duration = stepControlReturning(1.F);
            auto constexpr sampleTime{0.1F}; // Enough to complete the step
            controls.showInactive = [](int s) {};

            SUBCASE("generates") {
              auto output{-992.3F};
              controls.setOutput = [&](float v) { output = v; };
              stepController.execute(lowGate, sampleTime);
              CHECK_EQ(output, controls.endLevel(step));
            }

            SUBCASE("reports complete") {
              auto result = stepController.execute(lowGate, sampleTime);
              CHECK_EQ(result, StepEvent::Completed);
            }

            SUBCASE("deactivates step") {
              auto deactivatedStep = -1;
              controls.showInactive = [&](int step) { deactivatedStep = step; };
              stepController.execute(lowGate, sampleTime);
              CHECK_EQ(deactivatedStep, step);
            }
          }

          SUBCASE("if curve does not complete") {
            timer.reset();
            controls.duration = stepControlReturning(1.F);
            auto constexpr sampleTime{0.1F}; // Not enough to complete the step

            SUBCASE("generates") {
              auto output{-441.3F};
              controls.setOutput = [&](float v) { output = v; };
              stepController.execute(lowGate, sampleTime);
              CHECK_EQ(output, controls.endLevel(step));
            }

            SUBCASE("reports generated") {
              auto const result = stepController.execute(lowGate, sampleTime);
              CHECK_EQ(result, StepEvent::Generated);
            }

            SUBCASE("leaves step active") {
              controls.showInactive = [](int s) { throw forbidden("showInactive", s); };
              stepController.execute(lowGate, sampleTime);
            }
          }
        }

        SUBCASE("when sustains on EOC") {}

        SUBCASE("generate") {
          // TODO: start source
          // TODO: track start source
          // TODO: snap start source
          // TODO: track start -> snap start
          // TODO: snap start -> track start

          // TODO: end source
          // TODO: track end source
          // TODO: snap end source
          // TODO: track end -> snap end
          // TODO: snap end -> track end
        }
      }
    }
  } // namespace step_controller
} // namespace curve_sequencer_2
} // namespace test
