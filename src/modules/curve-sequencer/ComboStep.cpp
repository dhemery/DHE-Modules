#include "modules/curve-sequencer/ComboStep.h"

#include "modules/components/Latch.h"
#include "modules/curve-sequencer/GenerateStep.h"
#include "modules/curve-sequencer/Step.h"
#include "modules/curve-sequencer/StepControls.h"
#include "modules/curve-sequencer/SustainStep.h"

namespace dhe {
namespace curve_sequencer {

  ComboStep::ComboStep(StepControls &controls, int stepIndex) :
      ComboStep{controls, stepIndex, new GenerateStep{controls, stepIndex}, new SustainStep{controls, stepIndex}} {}

  ComboStep::ComboStep(StepControls &controls, int stepIndex, Step *generateStep, Step *sustainStep) :
      controls{controls}, stepIndex{stepIndex}, generateStep{generateStep}, sustainStep{sustainStep} {}

  auto ComboStep::isAvailable() const -> bool {
    return controls.isEnabled(stepIndex) && (generateStep->isAvailable() || sustainStep->isAvailable());
  }

  auto ComboStep::process(Latch const &gateLatch, float sampleTime) -> State {
    if (generateStep->isAvailable()) {
      return generateStep->process(gateLatch, sampleTime);
    }
    if (sustainStep->isAvailable()) {
      return sustainStep->process(gateLatch, sampleTime);
    }
    return State::Terminated;
  }

} // namespace curve_sequencer
} // namespace dhe
