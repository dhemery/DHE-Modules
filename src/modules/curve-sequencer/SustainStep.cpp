#include "modules/curve-sequencer/SustainStep.h"

#include <modules/components/Latch.h>

namespace dhe {
namespace curve_sequencer {
  SustainStep::SustainStep(StepControls &controls, int stepIndex) : controls{controls}, stepIndex{stepIndex} {}

  auto SustainStep::isAvailable() const -> bool { return controls.sustainMode(stepIndex) != StepMode::Skip; }

  void SustainStep::process(Latch const &gateLatch, float) {
    if(controls.sustainMode(stepIndex) == StepMode::Rise) {
      if(gateLatch.isRise()) {
        controls.setSustaining(stepIndex, false);
        return;
      }
    }
    if(controls.sustainMode(stepIndex) == StepMode::Fall) {
      if(gateLatch.isFall()) {
        controls.setSustaining(stepIndex, false);
        return;
      }
    }
    controls.setSustaining(stepIndex, true);
  }

} // namespace curve_sequencer
} // namespace dhe
