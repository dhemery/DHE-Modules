#include "modules/curve-sequencer/SustainStep.h"

#include <modules/components/Latch.h>

namespace dhe {
namespace curve_sequencer {
  SustainStep::SustainStep(StepControls &controls, int stepIndex) : controls{controls}, stepIndex{stepIndex} {}

  auto SustainStep::isAvailable() const -> bool { return controls.sustainMode(stepIndex) != Mode::Skip; }

  auto SustainStep::process(Latch const &gateLatch, float /* ignored */) -> State {
    if (controls.sustainMode(stepIndex) == Mode::Rise) {
      if (gateLatch.isRise()) {
        controls.setSustaining(stepIndex, false);
        return State::Finished;
      }
    }
    if (controls.sustainMode(stepIndex) == Mode::Fall) {
      if (gateLatch.isFall()) {
        controls.setSustaining(stepIndex, false);
        return State::Finished;
      }
    }
    controls.setSustaining(stepIndex, true);
    return State::Running;
  }

} // namespace curve_sequencer
} // namespace dhe
