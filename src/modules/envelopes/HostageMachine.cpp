#include "modules/envelopes/HostageMachine.h"

#include <algorithm>

namespace dhe {

void HostageMachine::process(float sampleTime) {
  auto const newState = identifyState();
  if (state != newState) {
    enter(newState);
  }

  switch (state) {
  case Holding:
    advanceHold(sampleTime);
  case Sustaining:
  case Idle:
    sendOut(heldVoltage);
    break;
  case Deferring:
  case TrackingInput:
    sendOut(envelopeIn());
  }

  sendActive(state != Idle && state != TrackingInput);
  advanceEoc(sampleTime);
  sendEoc(isEoc);
}

auto HostageMachine::identifyState() -> HostageMachine::State {
  if (deferIsHigh()) {
    return Deferring;
  }
  if (isSustainMode()) {
    if (stageGateIsHigh()) {
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
} // namespace dhe

void HostageMachine::enter(HostageMachine::State newState) {
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

void HostageMachine::resetStageGate() { stageGateWasHigh = false; }

auto HostageMachine::stageGateRise() -> bool {
  auto const isHigh = stageGateIsHigh();
  auto const isRise = isHigh && !stageGateWasHigh;
  stageGateWasHigh = isHigh;
  return isRise;
}

void HostageMachine::resetHold() {
  heldVoltage = envelopeIn();
  holdPhase = 0.F;
}

void HostageMachine::advanceHold(float sampleTime) {
  if (holdPhase < 1.F) {
    holdPhase = std::min(1.F, holdPhase + sampleTime / duration());
    if (holdPhase == 1.F) {
      finishStage();
    }
  }
}

void HostageMachine::finishStage() {
  startEoc();
  enter(Idle);
}

void HostageMachine::startEoc() {
  isEoc = true;
  eocPhase = 0.F;
}

void HostageMachine::advanceEoc(float sampleTime) {
  if (eocPhase < 1.F) {
    eocPhase = std::min(1.F, eocPhase + sampleTime / 1e-3F);
    if (eocPhase == 1.F) {
      finishEoc();
    }
  }
}

void HostageMachine::finishEoc() { isEoc = false; }
} // namespace dhe
