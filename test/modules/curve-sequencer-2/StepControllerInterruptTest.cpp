#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "fake/Controls.h"
#include "modules/curve-sequencer-2/StepController.h"
#include "modules/curve-sequencer-2/TriggerMode.h"
#include "modules/curve-sequencer/StepEvent.h"

#include <doctest.h>
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

    TEST_CASE("curve_sequencer_2::StepController ON INT") {
      fake::Controls controls{};
      PhaseTimer timer{};

      StepController<fake::Controls> stepController{controls, timer};

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
    }
  } // namespace step_controller
} // namespace curve_sequencer_2
} // namespace test
