#pragma once

#include "events.h"

#include "components/cxmath.h"
#include "components/latch.h"
#include "components/range.h"
#include "signals/shape-signals.h"

namespace dhe {
namespace envelope {
template <typename Signals, typename Timer> class GenerateMode {
public:
  GenerateMode(Signals &signals, Timer &timer)
      : signals_{signals}, timer_{timer} {}

  auto execute(Latch const &retrigger, float sample_time) -> Event {
    if (retrigger.is_rise()) {
      initialize_curve();
    }
    auto const level = signals_.level();
    auto const curvature = signals_.curvature();
    auto const shape = signals_.shape();

    timer_.advance(sample_time / signals_.duration());
    auto const tapered_phase = Shape::apply(timer_.phase(), shape, curvature);

    signals_.output(cx::scale(tapered_phase, start_voltage_, level));

    return timer_.in_progress() ? Event::Generated : Event::Completed;
  }

  void enter() {
    signals_.show_active(true);
    initialize_curve();
  }

  void exit() { signals_.show_active(false); }

private:
  void initialize_curve() {
    start_voltage_ = signals_.input();
    timer_.reset();
  }

  float start_voltage_{0.F};
  Signals &signals_;
  Timer &timer_;
};
} // namespace envelope
} // namespace dhe
