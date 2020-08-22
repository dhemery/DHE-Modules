#pragma once

#include "components/latch.h"
#include "components/phase-timer.h"
#include "event.h"

namespace dhe {
namespace stage {
template <typename Controls> class HoldMode {
public:
  HoldMode(Controls &controls, PhaseTimer &timer)
      : controls_{controls}, timer_{timer} {}

  auto execute(Latch const &retrigger, float sample_time) -> Event {
    if (retrigger.is_rise()) {
      reset();
    }
    timer_.advance(sample_time / controls_.duration());
    return timer_.in_progress() ? Event::Generated : Event::Completed;
  }
  void enter() {
    controls_.show_active(true);
    reset();
  }

  void exit() { controls_.show_active(false); }

private:
  void reset() const {
    controls_.output(controls_.input());
    timer_.reset();
  }

  Controls &controls_;
  PhaseTimer &timer_;
};
} // namespace stage
} // namespace dhe
