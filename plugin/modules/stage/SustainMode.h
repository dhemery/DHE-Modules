#pragma once

#include "Event.h"
#include "components/Latch.h"
#include "components/PhaseTimer.h"

namespace dhe {
namespace stage {
  template <typename Controls> class SustainMode {
  public:
    SustainMode(Controls &controls) : controls{controls} {}

    auto execute(Latch const &gate) -> Event { return gate.isHigh() ? Event::Generated : Event::Completed; }

    void enter() { controls.showActive(true); }

    void exit() { controls.showActive(false); }

  private:
    Controls &controls;
  };
} // namespace stage
} // namespace dhe
