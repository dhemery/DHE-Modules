#pragma once

#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer_2 {
  using dhe::Latch;

  template <typename Controls> class Sustainer {
  public:
    Sustainer(Controls &controls) : controls{controls} {}
    auto isDone(int step, Latch const &latch) -> bool {
      if (controls.advanceOnEndOfCurve(step)) {
        return true;
      }
      if (isTriggered(controls.triggerMode(step), latch)) {
        return true;
      }
      return false;
    }

  private:
    Controls &controls;
  };

} // namespace curve_sequencer_2
} // namespace dhe
