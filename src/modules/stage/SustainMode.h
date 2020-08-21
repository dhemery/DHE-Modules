#pragma once

#include "Event.h"
#include "components/latch.h"
#include "components/phase-timer.h"

namespace dhe {
namespace stage {
template <typename Controls> class SustainMode {
public:
  SustainMode(Controls &controls) : controls{controls} {}

  auto execute(Latch const &gate) -> Event {
    return gate.is_high() ? Event::Generated : Event::Completed;
  }

  void enter() { controls.showActive(true); }

  void exit() { controls.showActive(false); }

private:
  Controls &controls;
};
} // namespace stage
} // namespace dhe
