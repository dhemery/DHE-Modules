#pragma once

#include "Event.h"
#include "Mode.h"
#include "components/Latch.h"
#include "components/PhaseTimer.h"

#include <algorithm>

namespace dhe {
namespace stage {
  template <typename Controls, typename InputMode, typename DeferMode, typename HoldMode, typename SustainMode,
            typename IdleMode>
  class HostageEngine {
  public:
    HostageEngine(Controls &controls, InputMode &inputMode, DeferMode &deferMode, HoldMode &holdMode,
                  SustainMode &sustainMode, IdleMode &idleMode) :
        controls{controls},
        inputMode{inputMode},
        deferMode{deferMode},
        holdMode{holdMode},
        sustainMode{sustainMode},
        idleMode{idleMode} {}

    void process(float sampleTime) {
      defer.clock(controls.defer());
      gate.clock(controls.gate());

      auto const newState = identifyState();

      if (state != newState) {
        enter(newState);
      }

      switch (state) {
      case Mode::Defer:
        deferMode.execute();
        break;
      case Mode::Hold:
        if (holdMode.execute(gate, sampleTime) == Event::Completed) {
          enter(Mode::Idle);
        }
        break;
      case Mode::Idle:
        idleMode.execute();
        break;
      case Mode::Sustain:
        if (sustainMode.execute(gate) == Event::Completed) {
          enter(Mode::Idle);
        }
        break;
      case Mode::TrackInput:
        inputMode.execute();
        break;
      default:
        break;
      }

      controls.showActive(state != Mode::Idle && state != Mode::TrackInput);
      eocTimer.advance(sampleTime / controls.duration());
      controls.showEoc(eocTimer.inProgress());
    }

  private:
    auto identifyState() -> Mode {
      if (defer.isHigh()) { // DEFER trumps all
        return Mode::Defer;
      }
      if (controls.mode() == Mode::Sustain) { // Sustain mode is selected
        if (gate.isHigh()) {                  // If the GATE is high, start or continue sustaining
          return Mode::Sustain;
        }
        // Sustain mode is selected and GATE is low
        if (state == Mode::Sustain) { // If already sustaining, go idle
          return Mode::Idle;
        }
      }
      if (defer.isFall()) {
        return Mode::TrackInput;
      }
      if (gate.isRise()) {
        return Mode::Hold;
      }
      return state;
    }

    void enter(Mode newState) {
      switch (state) {
      case Mode::Defer:
        deferMode.exit();
        gate.clock(false);
        break;
      case Mode::Hold:
        holdMode.exit();
        break;
      case Mode::Idle:
        idleMode.exit();
        break;
      case Mode::Sustain:
        sustainMode.exit();
        break;
      case Mode::TrackInput:
        inputMode.exit();
        break;
      default:
        break;
      }

      state = newState;

      switch (state) {
      case Mode::Defer:
        deferMode.enter();
        gate.clock(false);
        break;
      case Mode::Hold:
        holdMode.enter();
        break;
      case Mode::Idle:
        idleMode.enter();
        eocTimer.reset();
        break;
      case Mode::Sustain:
        sustainMode.enter();
        break;
      case Mode::TrackInput:
        inputMode.enter();
        break;
      default:
        break;
      }
    }

    Mode state{Mode::TrackInput};
    PhaseTimer eocTimer{1.F};
    Latch defer{};
    Latch gate{};
    Controls &controls;
    InputMode &inputMode;
    DeferMode &deferMode;
    HoldMode &holdMode;
    SustainMode &sustainMode;
    IdleMode &idleMode;
  };
} // namespace stage
} // namespace dhe
