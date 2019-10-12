#pragma once

#include "Step.h"
#include "StepControls.h"

namespace dhe {
namespace curve_sequencer {
  class SustainStep : public Step {
  public:
    enum class Mode { Rise, Fall, Edge, High, Low, Skip };
    static auto constexpr modeCount = static_cast<int>(Mode::Skip) + 1;
    static auto constexpr defaultMode = static_cast<int>(Mode::Skip);

    SustainStep(StepControls &controls, int stepIndex);

    auto index() const -> int override { return stepIndex; }
    auto isAvailable() const -> bool override;
    auto process(Latch const &gateLatch, float sampleTime) -> State override;

  private:
    auto mode() const -> Mode { return static_cast<Mode>(controls.sustainMode(stepIndex)); }

    StepControls &controls;
    int stepIndex;
  };

} // namespace curve_sequencer
} // namespace dhe