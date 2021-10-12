#pragma once

#include "./event.h"
#include "components/latch.h"
#include "components/phase-timer.h"

namespace dhe {
namespace stage {
template <typename Controls> class SustainMode {
public:
  SustainMode(Controls &controls) : controls_{controls} {}

  auto execute(Latch const &gate) -> Event {
    return gate.is_high() ? Event::Generated : Event::Completed;
  }

  void enter() { controls_.show_active(true); }

  void exit() { controls_.show_active(false); }

private:
  Controls &controls_;
};
} // namespace stage
} // namespace dhe
