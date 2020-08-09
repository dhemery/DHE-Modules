#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "fake/Controls.h"
#include "fake/Interrupter.h"
#include "fake/Sustainer.h"
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
    using dhe::curve_sequencer_2::TriggerMode;
    using test::fake::Controls;
    using test::fake::forbidden;
    using test::fake::funcReturning;
    using test::fake::Interrupter;
    using test::fake::Sustainer;
    using StepController = dhe::curve_sequencer_2::StepController<Controls, Interrupter, Sustainer>;

    static auto constexpr risingGate = Latch{true, true};
    static auto constexpr fallingGate = Latch{false, true};
    static auto constexpr highGate = Latch{true, false};
    static auto constexpr lowGate = Latch{false, false};

    auto const gateStates = std::vector<Latch>{risingGate, fallingGate, lowGate, highGate};

    auto const triggerModes = std::vector<TriggerMode>{
        TriggerMode::GateRises,  TriggerMode::GateFalls, TriggerMode::GateChanges,
        TriggerMode::GateIsHigh, TriggerMode::GateIsLow,
    };

    TEST_CASE("curve_sequencer_2::StepController execute") {
      Controls controls{};
      Interrupter interrupter{};
      Sustainer sustainer{};
      PhaseTimer timer{};

      StepController stepController{controls, interrupter, sustainer, timer};

      SUBCASE("completes without generating if interrupted") {
        interrupter.isInterrupted = funcReturning<int,Latch const&>(true);
        auto constexpr step{7};
        controls.showProgress = [](int s, float p) {};
        stepController.enter(step);
        controls.showProgress = [](int s, float f) { throw forbidden("showProgress", s, f); };

        controls.showInactive = [](int s) {};
        auto result = stepController.execute(risingGate, 0.F);
        CHECK_EQ(result, StepEvent::Completed);
      }

      SUBCASE("generates if not interrupted") {
        interrupter.isInterrupted = funcReturning<int,Latch const&>(false);
        allowGenerate(controls);

        auto result = stepController.execute(Latch{}, 0.F);
        CHECK_EQ(result, StepEvent::Generated);
      }
    }
  } // namespace step_controller
} // namespace curve_sequencer_2
} // namespace test
