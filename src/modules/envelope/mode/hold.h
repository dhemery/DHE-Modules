#pragma once

#include "event.h"

#include "components/latch.h"
#include "components/phase-timer.h"

namespace dhe {
namespace envelope {
namespace mode {
template <typename Signals, typename Timer> class HoldMode {
public:
  HoldMode(Signals &signals, Timer &timer) : signals_{signals}, timer_{timer} {}

  auto execute(Latch const &retrigger, float sample_time) -> Event {
    if (retrigger.is_rise()) {
      reset();
    }
    timer_.advance(sample_time / signals_.duration());
    return timer_.in_progress() ? Event::Generated : Event::Completed;
  }
  void enter() {
    signals_.show_active(true);
    reset();
  }

  void exit() { signals_.show_active(false); }

private:
  void reset() const {
    signals_.output(signals_.input());
    timer_.reset();
  }

  Signals &signals_;
  Timer &timer_;
};
} // namespace mode
} // namespace envelope
} // namespace dhe
