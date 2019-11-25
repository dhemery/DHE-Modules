#pragma once

#include "Event.h"
#include "components/Latch.h"
#include "components/PhaseTimer.h"

#include <algorithm>

namespace dhe {
namespace stage {
  template <typename Controls, typename InputMode, typename DeferMode, typename HoldMode, typename IdleMode>
  class HostageEngine {
  private:
    enum State {
      Deferring,
      Holding,
      Idle,
      Sustaining,
      TrackingInput,
    };

  public:
    HostageEngine(Controls &controls, InputMode &inputMode, DeferMode &deferMode, HoldMode &holdMode,
                  IdleMode &idleMode) :
        controls{controls}, inputMode{inputMode}, deferMode{deferMode}, holdMode{holdMode}, idleMode{idleMode} {}

    void process(float sampleTime) {
      defer.clock(controls.defer());
      gate.clock(controls.gate());

      auto const newState = identifyState();

      if (state != newState) {
        enter(newState);
      }

      switch (state) {
      case Deferring:
        deferMode.execute();
        break;
      case Holding:
        hold(sampleTime);
      case Idle:
        idleMode.execute();
        break;
      case Sustaining:
        break;
      case TrackingInput:
        inputMode.execute();
        break;
      }

      controls.showActive(state != Idle && state != TrackingInput);
      eocTimer.advance(sampleTime / controls.duration());
      controls.showEoc(eocTimer.inProgress());
    }

  private:
    auto identifyState() -> State {
      if (defer.isHigh()) {
        return Deferring;
      }
      if (controls.isSustainMode()) {
        if (gate.isHigh()) {
          return Sustaining;
        }
        if (state == Sustaining) {
          return Idle;
        }
      }
      if (state == Deferring) {
        return TrackingInput;
      }
      if (gate.isRise()) {
        enter(Holding);
        return Holding;
      }
      return state;
    }

    void enter(State newState) {
      switch (state) {
      case Deferring:
        deferMode.exit();
        gate.clock(false);
        break;
      case Holding:
        holdMode.exit();
        break;
      case Idle:
        idleMode.exit();
        break;
      case Sustaining:
        break;
      case TrackingInput:
        inputMode.exit();
        break;
      }

      state = newState;

      switch (state) {
      case Deferring:
        deferMode.enter();
        break;
      case Holding:
        holdMode.enter();
        break;
      case Idle:
        idleMode.enter();
        break;
      case Sustaining:
        break;
      case TrackingInput:
        inputMode.enter();
        break;
      }
    }

    void hold(float sampleTime) {
      auto const event = holdMode.execute(gate, sampleTime);
      if (event == dhe::stage::Event::Completed) {
        eocTimer.reset();
        enter(Idle);
      }
    }

    PhaseTimer eocTimer{1.F};
    State state{TrackingInput};
    Latch defer{};
    Latch gate{};
    Controls &controls;
    InputMode &inputMode;
    DeferMode &deferMode;
    HoldMode &holdMode;
    IdleMode &idleMode;
  };
} // namespace stage
} // namespace dhe
