#include "modules/curve-sequencer/GenerateStep.h"

#include "modules/curve-sequencer/StepControls.h"

namespace dhe {
namespace curve_sequencer {
  GenerateStep::GenerateStep(StepControls &controls, int stepIndex) : controls{controls}, stepIndex{stepIndex} {}

  auto GenerateStep::isAvailable() const -> bool { return controls.generateMode(stepIndex) != Mode::Skip; }

  auto GenerateStep::process(Latch const & /*gateLatch*/, float /*sampleTime*/) -> State {
    controls.setGenerating(stepIndex, true);
    return State::Running;
  }

} // namespace curve_sequencer
} // namespace dhe
