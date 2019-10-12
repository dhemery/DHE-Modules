#pragma once

#include "Step.h"
#include "StepControls.h"

#include <modules/components/LoopingPhaseAccumulator.h>
#include <modules/components/OneShotPhaseAccumulator.h>

namespace dhe {
namespace curve_sequencer {
  class GenerateStep : public Step {
  public:
    enum class Mode { Rise, Fall, Edge, High, Low, Skip, Duration };
    static auto constexpr modeCount = static_cast<int>(Mode::Duration) + 1;
    static auto constexpr defaultMode = static_cast<int>(Mode::Duration);

    GenerateStep(StepControls &controls, int stepIndex);

    auto index() const -> int override { return stepIndex; }
    auto isAvailable() const -> bool override;
    auto process(Latch const &gateLatch, float sampleTime) -> State override;

  private:
    auto mode() const -> Mode { return static_cast<Mode>(controls.generateMode(stepIndex)); }
    auto generate(float sampleTime) -> State;
    auto taper(float phase) const -> float;

    StepControls &controls;
    int stepIndex;
    OneShotPhaseAccumulator ramp;
  };
} // namespace curve_sequencer
} // namespace dhe