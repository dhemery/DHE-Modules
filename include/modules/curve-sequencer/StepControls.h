#pragma once

#include "Step.h"
#include "modules/components/Taper.h"

namespace dhe {
namespace curve_sequencer {
  class StepControls {
  public:
    virtual auto curvature(int stepIndex) const -> float = 0;
    virtual auto duration(int stepIndex) const -> float = 0;
    virtual auto isEnabled(int stepIndex) const -> bool = 0;
    virtual auto generateMode(int stepIndex) const -> int = 0;
    virtual auto level(int stepIndex) const -> float = 0;
    virtual auto output() const -> float = 0;
    virtual auto sustainMode(int stepIndex) const -> int = 0;
    virtual void setGenerating(int stepIndex, bool state) = 0;
    virtual void setSustaining(int stepIndex, bool state) = 0;
    virtual void setOutput(float voltage) = 0;
    virtual auto taperSelection(int stepIndex) const -> int = 0;
  };

} // namespace curve_sequencer
} // namespace dhe
