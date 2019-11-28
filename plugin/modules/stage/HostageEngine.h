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
      gate.clock(controls.gate() && !defer.isHigh());

      auto const newMode = identifyMode();

      if (mode != newMode) {
        enter(newMode);
      }

      switch (mode) {
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
      case Mode::Input:
        inputMode.execute();
        break;
      default:
        break;
      }

      eocTimer.advance(sampleTime / 1e-3F);
      controls.showEoc(eocTimer.inProgress());
    }

  private:
    auto identifyMode() -> Mode {
      if (defer.isHigh()) { // DEFER trumps all
        return Mode::Defer;
      }
      switch (mode) {
      case Mode::Defer: // Leaving Defer mode
        if (controls.mode() == Mode::Hold) {
          return gate.isRise() ? Mode::Hold : Mode::Input;
        }
        return gate.isHigh() ? Mode::Sustain : Mode::Idle;
      case Mode::Idle: // Fall through to Mode::Input
      case Mode::Input:
        return gate.isRise() ? controls.mode() : mode;
      case Mode::Hold:
      case Mode::Sustain:
      default:
        return mode;
      }
    }

    void enter(Mode newMode) {
      switch (mode) {
      case Mode::Defer:
        deferMode.exit();
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
      case Mode::Input:
        inputMode.exit();
        break;
      default:
        break;
      }

      mode = newMode;

      switch (mode) {
      case Mode::Defer:
        deferMode.enter();
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
      case Mode::Input:
        inputMode.enter();
        break;
      default:
        break;
      }
    }

    Mode mode{Mode::Input};
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
