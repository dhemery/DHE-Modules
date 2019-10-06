#pragma once

#include "Step.h"
#include "StepControls.h"

namespace dhe {
namespace curve_sequencer {
  class SustainStep : public Step {
  public:
    SustainStep(StepControls &controls, int stepIndex);
    auto isAvailable() const -> bool override;
    auto process(Latch const &gateLatch, float sampleTime) -> State override;

  private:
    auto mode() const -> Mode { return static_cast<Mode>(controls.sustainMode(stepIndex)); }
    auto process(bool ends) -> State;

    StepControls &controls;
    int stepIndex;
  };

} // namespace curve_sequencer
} // namespace dhe