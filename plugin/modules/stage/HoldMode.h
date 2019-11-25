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
        initializeHold();
      }
      timer.advance(sampleTime / controls.duration());
      return timer.inProgress() ? Event::Generated : Event::Completed;
    }

    void enter() {
      controls.showActive(true);
      initializeHold();
    }

    void exit() { controls.showActive(false); }

  private:
    void initializeHold() {
      startVoltage = controls.input();
      timer.reset();
    }

    float startVoltage{0.F};
    Controls &controls;
    PhaseTimer &timer;
  };
} // namespace stage
} // namespace dhe
