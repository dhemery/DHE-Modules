#pragma once

#include "./event.h"
#include "components/latch.h"
#include "components/phase-timer.h"

namespace dhe {
namespace envelope {
namespace mode {
template <typename Signals> class SustainMode {
public:
  SustainMode(Signals &signals) : signals_{signals} {}

  auto execute(Latch const &gate) -> Event {
    return gate.is_high() ? Event::Generated : Event::Completed;
  }

  void enter() { signals_.show_active(true); }

  void exit() { signals_.show_active(false); }

private:
  Signals &signals_;
};
} // namespace mode
} // namespace envelope
} // namespace dhe
