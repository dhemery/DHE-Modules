#pragma once

#include "Step.h"

namespace dhe {
namespace curve_sequencer {
  class StepControls {
  public:
    virtual auto isEnabled(int stepIndex) const -> bool = 0;
    virtual auto generateMode(int stepIndex) const -> int = 0;
    virtual auto sustainMode(int stepIndex) const -> int = 0;
    virtual void setGenerating(int stepIndex, bool state) = 0;
    virtual void setSustaining(int stepIndex, bool state) = 0;
  };

} // namespace curve_sequencer
} // namespace dhe
