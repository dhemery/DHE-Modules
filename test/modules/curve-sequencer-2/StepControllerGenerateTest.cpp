#include "components/PhaseTimer.h"
#include "fake/Controls.h"
#include "modules/curve-sequencer-2/StepController.h"

#include <doctest.h>
#include <modules/curve-sequencer-2/fake/Interrupter.h>
#include <modules/curve-sequencer-2/fake/Sustainer.h>

namespace test {
namespace curve_sequencer_2 {
  namespace step_controller {
    using dhe::Latch;
    using dhe::PhaseTimer;
    using dhe::curve_sequencer::StepEvent;
    using dhe::curve_sequencer_2::TriggerMode;
    using test::fake::Interrupter;
    using test::fake::Sustainer;
    using StepController = dhe::curve_sequencer_2::StepController<fake::Controls, Interrupter, Sustainer>;

    TEST_CASE("curve_sequencer_2::StepController generate") {
      fake::Controls controls{};
      Interrupter interrupter{};
      Sustainer sustainer{};
      PhaseTimer timer{};

      StepController stepController{controls, interrupter, sustainer, timer};

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
  } // namespace step_controller
} // namespace curve_sequencer_2
} // namespace test
