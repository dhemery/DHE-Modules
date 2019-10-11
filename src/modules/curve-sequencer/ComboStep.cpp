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
    if (activeStage != nullptr) {
      return true;
    }
    if (!controls.isEnabled(stepIndex)) {
      return false;
    }
    return generateStep->isAvailable() || sustainStep->isAvailable();
  }

  auto ComboStep::process(Latch const &gateLatch, float sampleTime) -> State {
    if (activeStage == nullptr) {
      if (generateStep->isAvailable()) {
        activeStage = generateStep.get();
      } else {
        activeStage = sustainStep.get();
      }
    }
    auto const state = activeStage->process(gateLatch, sampleTime);
    if (state == State::Inactive) {
      if (activeStage == generateStep.get() && sustainStep->isAvailable()) {
        activeStage = sustainStep.get();
        return State::Active;
      }
      activeStage = nullptr;
    }
    return state;
  }
} // namespace curve_sequencer
} // namespace dhe
