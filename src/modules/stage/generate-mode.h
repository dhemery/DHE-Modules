#pragma once

#include "components/cxmath.h"
#include "components/latch.h"
#include "components/range.h"
#include "event.h"

namespace dhe {
namespace stage {
template <typename Controls, typename Timer> class GenerateMode {
public:
  GenerateMode(Controls &controls, Timer &timer)
      : controls_{controls}, timer_{timer} {}

  auto execute(Latch const &retrigger, float sample_time) -> Event {
    if (retrigger.is_rise()) {
      initialize_curve();
    }
    auto const level = controls_.level();
    auto const curvature = controls_.curvature();
    auto const &taper = controls_.taper();

    timer_.advance(sample_time / controls_.duration());
    auto const tapered_phase = taper.apply(timer_.phase(), curvature);

    controls_.output(cx::scale(tapered_phase, start_voltage_, level));

    return timer_.in_progress() ? Event::Generated : Event::Completed;
  }

  void enter() {
    controls_.show_active(true);
    initialize_curve();
  }

  void exit() { controls_.show_active(false); }

private:
  void initialize_curve() {
    start_voltage_ = controls_.input();
    timer_.reset();
  }

  float start_voltage_{0.F};
  Controls &controls_;
  Timer &timer_;
};
} // namespace stage
} // namespace dhe
