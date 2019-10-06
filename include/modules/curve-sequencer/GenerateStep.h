#pragma once

#include "Step.h"
#include "StepControls.h"

namespace dhe {
namespace curve_sequencer {
  class GenerateStep : public Step {
  public:
    GenerateStep(StepControls &controls, int stepIndex);
    auto isAvailable() const -> bool override;
    auto process(Latch const &gateLatch, float sampleTime) -> State override;

  private:
    auto mode() const -> Mode { return static_cast<Mode>(controls.generateMode(stepIndex)); }

    StepControls &controls;
    int stepIndex;
  };
} // namespace curve_sequencer
} // namespace dhe