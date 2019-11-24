#pragma once

#include "components/Range.h"

namespace dhe {
namespace stage {
  template <typename Controls, typename Timer> class GenerateMode {
  public:
    enum class Result { Generated, Completed };
    GenerateMode(Controls &controls, Timer &timer) : controls{controls}, timer{timer} {}

    auto execute(float sampleTime) -> Result {
      auto const level = controls.level();
      auto const curvature = controls.curvature();
      auto const *taper = controls.taper();

      timer.advance(sampleTime / controls.duration());
      auto const taperedPhase = taper->apply(timer.phase(), curvature);

      controls.output(scale(taperedPhase, startVoltage, level));

      return timer.isExpired() ? Result::Completed : Result::Generated;
    }

    void enter(float startVoltage) {
      this->startVoltage = startVoltage;
      controls.showActive(true);
    }

    void exit() { controls.showActive(false); }

  private:
    float startVoltage{0.F};
    Controls &controls;
    Timer &timer;
  };
} // namespace stage
}