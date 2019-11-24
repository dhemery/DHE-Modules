#pragma once

#include "Event.h"
#include "components/Range.h"

namespace dhe {
namespace stage {
  template <typename Controls, typename Timer> class GenerateMode {
  public:
    GenerateMode(Controls &controls, Timer &timer) : controls{controls}, timer{timer} {}

    auto execute(float sampleTime) -> Event {
      auto const level = controls.level();
      auto const curvature = controls.curvature();
      auto const *taper = controls.taper();

      timer.advance(sampleTime / controls.duration());
      auto const taperedPhase = taper->apply(timer.phase(), curvature);

      controls.output(scale(taperedPhase, startVoltage, level));

      return timer.isExpired() ? Event::Completed : Event::Generated;
    }

    void enter(float curveStartingVoltage) {
      startVoltage = curveStartingVoltage;
      controls.showActive(true);
      timer.reset();
    }

    void exit() { controls.showActive(false); }

  private:
    float startVoltage{0.F};
    Controls &controls;
    Timer &timer;
  };
} // namespace stage
}