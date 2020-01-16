#pragma once

#include "Event.h"
#include "components/Latch.h"
#include "components/Range.h"

namespace dhe {
namespace stage {
  template <typename Controls, typename Timer> class GenerateMode {
  public:
    GenerateMode(Controls &controls, Timer &timer) : controls{controls}, timer{timer} {}

    auto execute(Latch const &retrigger, float sampleTime) -> Event {
      if (retrigger.isRise()) {
        initializeCurve();
      }
      auto const level = controls.level();
      auto const curvature = controls.curvature();
      auto const *taper = controls.taper();

      timer.advance(sampleTime / controls.duration());
      auto const taperedPhase = taper->apply(timer.phase(), curvature);

      controls.output(scale(taperedPhase, startVoltage, level));

      return timer.inProgress() ? Event::Generated : Event::Completed;
    }

    void enter() {
      controls.showActive(true);
      initializeCurve();
    }

    void exit() { controls.showActive(false); }

  private:
    void initializeCurve() {
      startVoltage = controls.input();
      timer.reset();
    }

    float startVoltage{0.F};
    Controls &controls;
    Timer &timer;
  };
} // namespace stage
} // namespace dhe
