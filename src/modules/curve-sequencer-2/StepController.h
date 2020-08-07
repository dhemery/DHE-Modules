#pragma once

#include "TriggerMode.h"
#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "components/Range.h"
#include "modules/curve-sequencer/StepEvent.h"

namespace dhe {
namespace curve_sequencer_2 {

  using dhe::Latch;
  using dhe::PhaseTimer;
  using dhe::curve_sequencer::StepEvent;

  static inline auto isTriggered(TriggerMode triggerMode, Latch const &gate) -> bool {
    switch (triggerMode) {
    case TriggerMode::GateIsHigh:
      return gate.isHigh();
    case TriggerMode::GateIsLow:
      return gate.isLow();
    case TriggerMode::GateRises:
      return gate.isRise();
    case TriggerMode::GateFalls:
      return gate.isFall();
    case TriggerMode::GateChanges:
      return gate.isEdge();
    }
  }

  template <typename Controls> class StepController {
  public:
    StepController(Controls &controls, PhaseTimer &timer) : controls{controls}, timer{timer} {}

    void enter(int entryStep) {
      step = entryStep;
      reset();
      controls.showProgress(step, 0.F);
    }

    auto execute(Latch const &gateLatch, float sampleTime) -> StepEvent { return StepEvent::Completed; };

    void exit() { controls.showInactive(step); }

  private:
    auto duration() const -> float { return controls.duration(step); }

    auto level() const -> float { return controls.level(step); }

    void reset() {
      timer.reset();
      startVoltage = controls.output();
    }

    auto taper(float input) const -> float {
      auto const curvature = controls.curvature(step);
      auto const taper = controls.taper(step);
      return taper->apply(input, curvature);
    }

    int step{0};
    float startVoltage{0.F};
    Controls &controls;
    PhaseTimer &timer;
  }; // namespace curve_sequencer
} // namespace curve_sequencer_2
} // namespace dhe
