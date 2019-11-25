#pragma once

#include "Event.h"
#include "components/Latch.h"
#include "components/PhaseTimer.h"

namespace dhe {
namespace stage {
  template <typename Controls> class HoldMode {
  public:
    HoldMode(Controls &controls, PhaseTimer &timer) : controls{controls}, timer{timer} {}

    auto execute(Latch const &retrigger, float sampleTime) -> Event {
      if (retrigger.isRise()) {
        timer.reset();
      }
      timer.advance(sampleTime / controls.duration());
      return timer.inProgress() ? Event::Generated : Event::Completed;
    }

    void enter() {
      controls.showActive(true);
      timer.reset();
    }

    void exit() { controls.showActive(false); }

  private:
    Controls &controls;
    PhaseTimer &timer;
  };
} // namespace stage
} // namespace dhe