#pragma once

#include <algorithm>

namespace dhe {
namespace stage {
  template <typename Controls> class HostageEngine {
  private:
    enum State {
      Deferring,
      Holding,
      Sustaining,
      Idle,
      TrackingInput,
    };

  public:
    HostageEngine(Controls &controls) : controls{controls} {}

    void process(float sampleTime) {
      auto const newState = identifyState();
      if (state != newState) {
        enter(newState);
      }

      switch (state) {
      case Holding:
        advanceHold(sampleTime);
      case Sustaining:
      case Idle:
        controls.sendOut(heldVoltage);
        break;
      case Deferring:
      case TrackingInput:
        controls.sendOut(controls.envelopeIn());
      }

      controls.sendActive(state != Idle && state != TrackingInput);
      advanceEoc(sampleTime);
      controls.sendEoc(isEoc);
    }

  private:
    auto identifyState() -> State {
      if (controls.isDeferring()) {
        return Deferring;
      }
      if (controls.isSustainMode()) {
        if (controls.isGated()) {
          return Sustaining;
        }
        if (state == Sustaining) {
          return Idle;
        }
      }
      if (state == Deferring) {
        return TrackingInput;
      }
      if (stageGateRise()) {
        enter(Holding);
        return Holding;
      }
      return state;
    }

    void enter(State newState) {
      if (state == Deferring) {
        resetStageGate();
      }

      if (newState == Holding || newState == Sustaining) {
        resetHold();
      }

      if (newState == Idle) {
        startEoc();
      }
      state = newState;
    }

    void resetStageGate() { stageGateWasHigh = false; }

    auto stageGateRise() -> bool {
      auto const isHigh = controls.isGated();
      auto const isRise = isHigh && !stageGateWasHigh;
      stageGateWasHigh = isHigh;
      return isRise;
    }

    void resetHold() {
      heldVoltage = controls.envelopeIn();
      holdPhase = 0.F;
    }

    void advanceHold(float sampleTime) {
      if (holdPhase < 1.F) {
        holdPhase = std::min(1.F, holdPhase + sampleTime / controls.duration());
        if (holdPhase == 1.F) {
          finishStage();
        }
      }
    }

    void finishStage() {
      startEoc();
      enter(Idle);
    }

    void startEoc() {
      isEoc = true;
      eocPhase = 0.F;
    }

    void advanceEoc(float sampleTime) {
      if (eocPhase < 1.F) {
        eocPhase = std::min(1.F, eocPhase + sampleTime / 1e-3F);
        if (eocPhase == 1.F) {
          finishEoc();
        }
      }
    }

    void finishEoc() { isEoc = false; }

    float eocPhase{1.F};
    bool isEoc{false};
    float holdPhase{0.F};
    float heldVoltage{0.F};
    State state{TrackingInput};
    bool stageGateWasHigh{false};
    Controls &controls;
  };
} // namespace stage
} // namespace dhe
