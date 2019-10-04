#include "modules/curve-sequencer/GenerateStep.h"

#include "modules/curve-sequencer/StepControls.h"

namespace dhe {
namespace curve_sequencer {
  GenerateStep::GenerateStep(StepControls &controls, int stepIndex) : controls{controls}, stepIndex{stepIndex} {}

  auto GenerateStep::isAvailable() const -> bool { return controls.generateMode(stepIndex) != StepMode::Skip; }

  void GenerateStep::process(float sampleTime) { controls.setGenerating(stepIndex, true); }

} // namespace curve_sequencer
} // namespace dhe
