#pragma once

#include "TriggerMode.h"
#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer_2 {
  using dhe::Latch;

  template <typename Controls> class Interrupter {
  public:
    Interrupter(Controls &controls) : controls{controls} {}

    auto isInterrupted(int step, Latch const &latch) -> bool {
      if (!controls.interruptOnTrigger(step)) {
        return false;
      }
      return isTriggered(controls.triggerMode(step), latch);
    }

  private:
    Controls &controls;
  };

} // namespace curve_sequencer_2
} // namespace dhe
