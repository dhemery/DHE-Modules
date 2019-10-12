#pragma once

#include "Step.h"
#include "StepControls.h"

#include <modules/components/LoopingPhaseAccumulator.h>

namespace dhe {
namespace curve_sequencer {
  class GenerateStep : public Step {
  public:
    GenerateStep(StepControls &controls, int stepIndex);

    auto index() const -> int override { return stepIndex; }
    auto isAvailable() const -> bool override;
    auto process(Latch const &gateLatch, float sampleTime) -> State override;

  private:
    auto mode() const -> Mode { return static_cast<Mode>(controls.generateMode(stepIndex)); }
    auto process(float sampleTime, bool isGenerating) -> State;

    StepControls &controls;
    int stepIndex;
  };
} // namespace curve_sequencer
} // namespace dhe