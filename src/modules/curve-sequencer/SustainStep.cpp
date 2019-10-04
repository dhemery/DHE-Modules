#include "modules/curve-sequencer/SustainStep.h"

namespace dhe {
namespace curve_sequencer {

  using Mode = StepControls::Mode;

  SustainStep::SustainStep(StepControls &controls, int stepIndex) : controls{controls}, stepIndex{stepIndex} {}

  auto SustainStep::isAvailable() const -> bool { return controls.sustainMode(stepIndex) != Mode::Skip; }

  void SustainStep::process(float sampleTime) { controls.setSustaining(stepIndex, true); }

} // namespace curve_sequencer
} // namespace dhe
