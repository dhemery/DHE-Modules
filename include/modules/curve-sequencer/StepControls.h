#pragma once

namespace dhe {
namespace curve_sequencer {
  class StepControls {
  public:
    virtual auto isEnabled(int step) const -> bool = 0;
    virtual auto generateMode(int step) const -> int = 0;
    virtual auto sustainMode(int step) const -> int = 0;
    virtual void setGenerating(int step, bool state) = 0;
    virtual void setSustaining(int step, bool state) = 0;
  };

} // namespace curve_sequencer
} // namespace dhe
